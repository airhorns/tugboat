#include "tugboat_config.h"
#include <Adafruit_MCP4725.h>

#define DEBUG_SERIAL    SerialBT2
#define DAC_LEFT_ID     0x63       // I2C address of left motor control DAC
#define DAC_RIGHT_ID    0x64       // I2C address of right motor control DAC

#define DAC_VOLTAGE_RANGE               1500 // For the 12 bit DACs go up or down this much from halfway (2.5V). 1500 is roughly 70% of 2048, which gives a max of 4.25V and a min of .75V, which were observed to be about right.
#define VELOCITY_CONSTANT_VALUE         0.05

class WheelchairMotorDriver {
 public:
  WheelchairMotorDriver();
  ~WheelchairMotorDriver();
  bool init(void);
  void close(void);
  bool readEncoder(int32_t &left_value, int32_t &right_value);
  bool writeVelocity(int64_t left_value, int64_t right_value);
  bool controlMotor(const float wheel_separation, float* value);

 private:
  Adafruit_MCP4725 left_wheel_dac_;
  Adafruit_MCP4725 right_wheel_dac_;
};