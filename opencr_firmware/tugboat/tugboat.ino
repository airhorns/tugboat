#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>

int led_pin_user[4] = { BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4 };

ros::NodeHandle  nh;

void messageCb( const std_msgs::Byte& led_msg) {
  int i;

  for (i=0; i<4; i++)
  {
    if (led_msg.data & (1<<i))
    {
      digitalWrite(led_pin_user[i], LOW);
    }
    else
    {
      digitalWrite(led_pin_user[i], HIGH);
    }
  }
}

ros::Subscriber<std_msgs::Byte> sub("led_out", messageCb );


void setup() {
  pinMode(led_pin_user[0], OUTPUT);
  pinMode(led_pin_user[1], OUTPUT);
  pinMode(led_pin_user[2], OUTPUT);
  pinMode(led_pin_user[3], OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
}
