#define ARDUINO 100
#include "src/tugboat_config.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include "src/wheelchair_motor_driver.h"

int led_pin_user[4] = { BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4 };

ros::NodeHandle  nh;
ros::Time current_time;
uint32_t current_offset;
static uint32_t tTime[10]; // software timer for various updates

WheelchairMotorDriver motor_driver;
float goal_velocity[WHEEL_NUM] = {0.0, 0.0};
float goal_velocity_from_button[WHEEL_NUM] = {0.0, 0.0};
float goal_velocity_from_cmd[WHEEL_NUM] = {0.0, 0.0};

void lightLedCallback( const std_msgs::Byte& led_msg) {
  int i;

  for (i = 0; i < 4; i++) {
    if (led_msg.data & (1 << i)) {
      digitalWrite(led_pin_user[i], LOW);
    } else {
      digitalWrite(led_pin_user[i], HIGH);
    }
  }
}

void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg) {
  goal_velocity_from_cmd[LINEAR]  = cmd_vel_msg.linear.x;
  goal_velocity_from_cmd[ANGULAR] = cmd_vel_msg.angular.z;

  goal_velocity_from_cmd[LINEAR]  = constrain(goal_velocity_from_cmd[LINEAR],  MIN_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);
  goal_velocity_from_cmd[ANGULAR] = constrain(goal_velocity_from_cmd[ANGULAR], MIN_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
};

void motorPowerCallback(const std_msgs::Bool& power_msg);
void resetCallback(const std_msgs::Empty& reset_msg);

ros::Subscriber<std_msgs::Byte> led_out_sub("led_out", lightLedCallback );
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", commandVelocityCallback);
ros::Subscriber<std_msgs::Bool> motor_power_sub("motor_power", motorPowerCallback);
ros::Subscriber<std_msgs::Empty> reset_sub("reset", resetCallback);

void setup() {
  motor_driver.init();

  pinMode(led_pin_user[0], OUTPUT);
  pinMode(led_pin_user[1], OUTPUT);
  pinMode(led_pin_user[2], OUTPUT);
  pinMode(led_pin_user[3], OUTPUT);

  nh.initNode();
  nh.subscribe(led_out_sub);
  nh.subscribe(cmd_vel_sub);
  nh.subscribe(motor_power_sub);
  nh.subscribe(reset_sub);
}

void updateGoalVelocity(void) {
  goal_velocity[LINEAR]  = goal_velocity_from_button[LINEAR]  + goal_velocity_from_cmd[LINEAR];
  goal_velocity[ANGULAR] = goal_velocity_from_button[ANGULAR] + goal_velocity_from_cmd[ANGULAR];
}

void updateTime() {
  current_offset = millis();
  current_time = nh.now();
}

void loop() {
  nh.spinOnce();

  uint32_t t = millis();
  updateTime();

  if ((t - tTime[0]) >= (1000 / CONTROL_MOTOR_SPEED_FREQUENCY)) {
    updateGoalVelocity();
    motor_driver.controlMotor(WHEEL_SEPARATION, goal_velocity);
    tTime[0] = t;
  }
}
