#include <Arduino.h>

#define LED_TXD                          0
#define LED_RXD                          1
#define LED_LOW_BATTERY                  2
#define LED_ROS_CONNECT                  3
#define LED_WORKING_CHECK                13

#define BATTERY_POWER_OFF                0
#define BATTERY_POWER_STARTUP            1
#define BATTERY_POWER_NORMAL             2
#define BATTERY_POWER_CHECK              3
#define BATTERY_POWER_WARNNING           4

#define WAIT_FOR_BUTTON_PRESS            0
#define WAIT_SECOND                      1
#define CHECK_BUTTON_RELEASED            2

class TugboatLEDDriver
{
 public:
  TugboatLEDDriver();

  bool init();

  void showLedStatus(bool isConnected);
  void updateRxTxLed(void);

  void setPowerOn(void);
  void setPowerOff(void);

  uint8_t updateVoltageCheck(bool check_setup);

  uint8_t getButtonPress(uint16_t time_to_press);  

 private:

};