#include <Arduino.h>
#define WAIT_FOR_BUTTON_PRESS            0
#define WAIT_SECOND                      1
#define CHECK_BUTTON_RELEASED            2

#define DEBUG_SERIAL  SerialBT2

class TugboatButtonDetector
{
 public:
  TugboatButtonDetector();

  uint8_t getButtonPress(uint16_t time_to_press);  

 private:

};