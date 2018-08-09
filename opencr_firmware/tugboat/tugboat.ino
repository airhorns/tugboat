#include "src/tugboat_config.h"
#define DEBUG 1
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include "src/wheelchair_motor_driver.h"
#include "src/tugboat_led_driver.h"
#include "src/tugboat_button_detector.h"

int led_pin_user[4] = { BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4 };

ros::NodeHandle  nh;
ros::Time current_time;
uint32_t current_offset;
static uint32_t tTime[10]; // software timer for various updates

WheelchairMotorDriver motor_driver;
float goal_velocity[2] = {0.0, 0.0};
float goal_velocity_from_button[2] = {0.0, 0.0};
float goal_velocity_from_cmd[2] = {0.0, 0.0};

TugboatLEDDriver led_driver;
TugboatButtonDetector button_detector;

char log_msg[150];

void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg) {
  goal_velocity_from_cmd[LINEAR]  = cmd_vel_msg.linear.x;
  goal_velocity_from_cmd[ANGULAR] = cmd_vel_msg.angular.z;
  sprintf(log_msg, "goal_velocity_cmd linear=%9.6f angular=%9.6f", cmd_vel_msg.linear.x, cmd_vel_msg.angular.z);
  nh.loginfo(log_msg); 
  goal_velocity_from_cmd[LINEAR]  = constrain(goal_velocity_from_cmd[LINEAR],  MIN_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);
  goal_velocity_from_cmd[ANGULAR] = constrain(goal_velocity_from_cmd[ANGULAR], MIN_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
};

void motorPowerCallback(const std_msgs::Bool& power_msg) {};
void resetCallback(const std_msgs::Empty& reset_msg) {};

ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", commandVelocityCallback);
ros::Subscriber<std_msgs::Bool> motor_power_sub("motor_power", motorPowerCallback);
ros::Subscriber<std_msgs::Empty> reset_sub("reset", resetCallback);

unsigned long prev_update_time;

void setup() {
  motor_driver.init();
  prev_update_time = millis();

  pinMode(led_pin_user[0], OUTPUT);
  pinMode(led_pin_user[1], OUTPUT);
  pinMode(led_pin_user[2], OUTPUT);
  pinMode(led_pin_user[3], OUTPUT);

  nh.initNode();
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

void waitForSerialLink(bool isConnected) {
  static bool wait_flag = false;
  
  if (isConnected) {
    if (wait_flag == false) {      
      delay(10);
      wait_flag = true;
    }
  } else {
    wait_flag = false;
  }
}

void driveTest(uint8_t buttons) {
  static bool move[2] = {false, false};

  if (buttons & (1<<0)) {
    move[LINEAR] = true;
  } else if (buttons & (1<<1)) {
    move[ANGULAR] = true;
  }

  if (move[LINEAR]) {    
    goal_velocity_from_button[LINEAR] = 0.05;
  } else {
    goal_velocity_from_button[LINEAR] = 0.0;
  } 
  
  if (move[ANGULAR]) {   
    goal_velocity_from_button[ANGULAR] = -0.7;
  } else {
    goal_velocity_from_button[ANGULAR] = 0.0;
  }
}


void loop() {
  uint32_t t = millis();
  updateTime();

  if (nh.connected() && (t - tTime[0]) >= (1000 / CONTROL_MOTOR_SPEED_FREQUENCY)) {
    updateGoalVelocity();

    motor_driver.controlMotor(WHEEL_SEPARATION, goal_velocity);
    tTime[0] = t;
  }

  // Light LEDs if ROS is connected or tx/rx is happening
  led_driver.showLedStatus(nh.connected());

  // Check push button pressed for simple test drive
  driveTest(button_detector.getButtonPress(3000));

  nh.spinOnce();

  // Wait the serial link time to process
  waitForSerialLink(nh.connected());
}
