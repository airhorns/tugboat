#!/usr/bin/env python
# license removed for brevity
import rospy
import math
from geometry_msgs.msg import Twist

def motor_range_loop():
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    rospy.init_node('motor_tester', anonymous=True)
    rate = rospy.Rate(10) # 10hz

    while not rospy.is_shutdown():
        seconds = rospy.get_rostime().to_sec()

        vel_msg = Twist()
        vel_msg.linear.x = math.sin(seconds / 2) / 5
        vel_msg.linear.y = 0
        vel_msg.linear.z = 0
        vel_msg.angular.x = 0
        vel_msg.angular.y = 0
        vel_msg.angular.z = math.sin(seconds / 3) * 1.3
        rospy.loginfo(vel_msg)
        pub.publish(vel_msg)
        rate.sleep()

if __name__ == '__main__':
    try:
        motor_range_loop()
    except rospy.ROSInterruptException:
        pass