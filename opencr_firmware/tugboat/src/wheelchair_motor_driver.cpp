#include <Wire.h>
#include "wheelchair_motor_driver.h"

WheelchairMotorDriver::WheelchairMotorDriver() {

}

WheelchairMotorDriver::~WheelchairMotorDriver() {
  close();
}

bool WheelchairMotorDriver::init(void) {
  DEBUG_SERIAL.begin(57600);
  left_wheel_dac_.begin(DAC_LEFT_ID);
  right_wheel_dac_.begin(DAC_RIGHT_ID);

  // Init at 2.5V to match the joystick's home position at 2.5V
  left_wheel_dac_.setVoltage(2048, false);
  right_wheel_dac_.setVoltage(2048, false);

  DEBUG_SERIAL.println("Success initing WheelchairMotorDriver");
  return true;
}

void WheelchairMotorDriver::close(void) {
  DEBUG_SERIAL.end();
}

bool WheelchairMotorDriver::readEncoder(int32_t &left_value, int32_t &right_value) {
  // left_value  = groupSyncReadEncoder_->getData(left_wheel_id_,  ADDR_X_PRESENT_POSITION, LEN_X_PRESENT_POSITION);
  // right_value = groupSyncReadEncoder_->getData(right_wheel_id_, ADDR_X_PRESENT_POSITION, LEN_X_PRESENT_POSITION);

  return true;
}

bool WheelchairMotorDriver::writeVelocity(int64_t left_value, int64_t right_value) {
  return true;
}

bool WheelchairMotorDriver::controlMotor(const float wheel_separation, float* value) {
  float wheel_velocity_cmd[2];

  float lin_vel = value[LEFT];
  float ang_vel = value[RIGHT];

  wheel_velocity_cmd[LEFT]   = lin_vel - (ang_vel * wheel_separation / 2);
  wheel_velocity_cmd[RIGHT]  = lin_vel + (ang_vel * wheel_separation / 2);

  wheel_velocity_cmd[LEFT]  = constrain(wheel_velocity_cmd[LEFT]  * VELOCITY_CONSTANT_VALUE, -DAC_VOLTAGE_RANGE, DAC_VOLTAGE_RANGE) + 2048; // convert to 12 bit scale for DAC voltage
  wheel_velocity_cmd[RIGHT] = constrain(wheel_velocity_cmd[RIGHT] * VELOCITY_CONSTANT_VALUE, -DAC_VOLTAGE_RANGE, DAC_VOLTAGE_RANGE) + 2048;

  left_wheel_dac_.setVoltage(wheel_velocity_cmd[LEFT], false);
  right_wheel_dac_.setVoltage(wheel_velocity_cmd[RIGHT], false);

  return true;
}