#include "tugboat_button_detector.h"

TugboatButtonDetector::TugboatButtonDetector()
{
}

uint8_t TugboatButtonDetector::getButtonPress(uint16_t time_to_press) {
  uint8_t button_state = 0;
  static uint32_t t_time[2];
  static uint8_t button_state_num[2] = {0, };

  for (int button_num = 0; button_num < 2; button_num++) {
    switch (button_state_num[button_num]) {
     case WAIT_FOR_BUTTON_PRESS:
       if (getPushButton() & (1 << button_num)) {
         t_time[button_num] = millis();
         button_state_num[button_num] = WAIT_SECOND;
       }
       break;

     case WAIT_SECOND:
       if ((millis()-t_time[button_num]) >= time_to_press) {
         if (getPushButton() & (1 << button_num)) {
           button_state_num[button_num] = CHECK_BUTTON_RELEASED;
           button_state |= (1 << button_num);
         } else {
           button_state_num[button_num] = WAIT_FOR_BUTTON_PRESS;
         }
       }
       break;

     case CHECK_BUTTON_RELEASED:
       if (!(getPushButton() & (1 << button_num)))
         button_state_num[button_num] = WAIT_FOR_BUTTON_PRESS;
       break;

     default :
       button_state_num[button_num] = WAIT_FOR_BUTTON_PRESS;
       break;
    }
  }

  return button_state;
}