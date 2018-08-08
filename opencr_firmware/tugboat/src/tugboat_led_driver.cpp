/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Yoonseok Pyo, Leon Jung, Darby Lim, HanCheol Cho */

#include "tugboat_led_driver.h"

TugboatLEDDriver::TugboatLEDDriver() {
}

void TugboatLEDDriver::showLedStatus(bool isConnected) {
  static uint32_t t_time = millis();

  if ((millis()-t_time) >= 500) {
    t_time = millis();
    digitalWrite(LED_WORKING_CHECK, !digitalRead(LED_WORKING_CHECK));
  }

  if (getPowerInVoltage() < 11.1) {
    setLedOn(LED_LOW_BATTERY);
  } else {
    setLedOff(LED_LOW_BATTERY);
  }

  if (isConnected) {
    setLedOn(LED_ROS_CONNECT);
  } else {
    setLedOff(LED_ROS_CONNECT);
  }

  updateRxTxLed();
}

void TugboatLEDDriver::updateRxTxLed(void)
{
  static uint32_t rx_led_update_time;
  static uint32_t tx_led_update_time;
  static uint32_t rx_cnt;
  static uint32_t tx_cnt;

  if ((millis()-tx_led_update_time) > 50) {
    tx_led_update_time = millis();

    if (tx_cnt != Serial.getTxCnt()) {
      setLedToggle(LED_TXD);
    } else {
      setLedOff(LED_TXD);
    }

    tx_cnt = Serial.getTxCnt();
  }

  if ((millis()-rx_led_update_time) > 50) {
    rx_led_update_time = millis();

    if (rx_cnt != Serial.getRxCnt()) {
      setLedToggle(LED_RXD);
    } else {
      setLedOff(LED_RXD);
    }

    rx_cnt = Serial.getRxCnt();
  }
}

uint8_t TugboatLEDDriver::updateVoltageCheck(bool check_setup) {  
  static uint8_t battery_voltage     = 0;
  static float   battery_valtage_raw = 0;
  static uint8_t battery_state       = BATTERY_POWER_OFF;

  static bool startup = false;
  static int vol_index = 0;
  static int prev_state = 0;
  static int alarm_state = 0;
  static int check_index = 0;

  int i;
  float vol_sum;
  float vol_value;

  static uint32_t process_time[8] = {0,};
  static float    vol_value_tbl[10] = {0,};

  float voltage_ref       = 11.0 + 0.0;
  float voltage_ref_warn  = 11.0 + 0.0;

  if (startup == false) {
    startup = true;
    for (i=0; i<8; i++) {
      process_time[i] = millis();
    }
  }

  if (millis()-process_time[0] > 100) {
    process_time[0] = millis();

    vol_value_tbl[vol_index] = getPowerInVoltage();

    vol_index++;
    vol_index %= 10;

    vol_sum = 0;
    for(i=0; i<10; i++) {
      vol_sum += vol_value_tbl[i];
    }
    vol_value = vol_sum/10;
    battery_valtage_raw = vol_value;
    battery_voltage = vol_value;
  }

  if(millis()-process_time[1] > 1000) {
    process_time[1] = millis();

    switch(battery_state) {
      case BATTERY_POWER_OFF:
        if (check_setup == true) {
          alarm_state = 0;
          if(battery_valtage_raw > 5.0) {
            check_index    = 0;
            prev_state     = battery_state;
            battery_state = BATTERY_POWER_STARTUP;
          } else {
            noTone(BDPIN_BUZZER);
          }
        }
        break;

      case BATTERY_POWER_STARTUP:
        if (battery_valtage_raw > voltage_ref) {
          check_index   = 0;
          prev_state    = battery_state;
          battery_state = BATTERY_POWER_NORMAL;
          setPowerOn();
        }

        if (check_index < 5) {
          check_index++;
        } else {
          if (battery_valtage_raw > 5.0) {
            prev_state    = battery_state;
            battery_state = BATTERY_POWER_CHECK;
          } else {
            prev_state    = battery_state;
            battery_state = BATTERY_POWER_OFF;
          }
        }
        break;

      case BATTERY_POWER_NORMAL:
        alarm_state = 0;
        if (battery_valtage_raw < voltage_ref) {
          prev_state    = battery_state;
          battery_state = BATTERY_POWER_CHECK;
          check_index   = 0;
        }
        break;

      case BATTERY_POWER_CHECK:
        if (check_index < 5) {
          check_index++;
        } else {
          if(battery_valtage_raw < voltage_ref_warn) {
            setPowerOff();
            prev_state    = battery_state;
            battery_state = BATTERY_POWER_WARNNING;
          }
        }
        if (battery_valtage_raw >= voltage_ref) {
          setPowerOn();
          prev_state    = battery_state;
          battery_state = BATTERY_POWER_NORMAL;
        }
        break;

      case BATTERY_POWER_WARNNING:
        alarm_state ^= 1;
        if(alarm_state) {
          tone(BDPIN_BUZZER, 1000, 500);
        }

        if(battery_valtage_raw > voltage_ref) {
          setPowerOn();
          prev_state    = battery_state;
          battery_state = BATTERY_POWER_NORMAL;
        } else {
          setPowerOff();
        }

        if(battery_valtage_raw < 5.0) {
          setPowerOff();
          prev_state    = battery_state;
          battery_state = BATTERY_POWER_OFF;
        }
        break;

      default:
        break;
    }
  }

  return battery_state;
}