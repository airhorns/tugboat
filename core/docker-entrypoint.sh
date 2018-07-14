#!/usr/bin/env bash
source /opt/ros/kinetic/setup.bash
source /root/catkin_ws/devel/setup.bash
export ROS_IP=$(python /root/resin-ip.py)
exec "$@"
