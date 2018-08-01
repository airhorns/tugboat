#define ARDUINO 100
#include "src/tugboat_config.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>
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

void messageCb( const std_msgs::Byte& led_msg) {
  int i;

  for (i = 0; i < 4; i++)
  {
    if (led_msg.data & (1 << i))
    {
      digitalWrite(led_pin_user[i], LOW);
    }
    else
    {
      digitalWrite(led_pin_user[i], HIGH);
    }
  }
}

void updateGoalVelocity(void) {
  goal_velocity[LINEAR]  = goal_velocity_from_button[LINEAR]  + goal_velocity_from_cmd[LINEAR];
  goal_velocity[ANGULAR] = goal_velocity_from_button[ANGULAR] + goal_velocity_from_cmd[ANGULAR];
}

void updateTime() {
  current_offset = millis();
  current_time = nh.now();
}

ros::Subscriber<std_msgs::Byte> sub("led_out", messageCb );

void setup() {
  motor_driver.init();

  pinMode(led_pin_user[0], OUTPUT);
  pinMode(led_pin_user[1], OUTPUT);
  pinMode(led_pin_user[2], OUTPUT);
  pinMode(led_pin_user[3], OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
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
