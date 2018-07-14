FROM arm32v7/ros:kinetic-ros-base

# Turtlebot3 ROS dependencies
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y curl vim python-rosinstall ros-kinetic-rosserial-python ros-kinetic-tf ros-kinetic-teleop-twist-keyboard ros-kinetic-laser-proc ros-kinetic-rgbd-launch ros-kinetic-depthimage-to-laserscan ros-kinetic-rosserial-arduino ros-kinetic-rosserial-python ros-kinetic-rosserial-server ros-kinetic-rosserial-client ros-kinetic-rosserial-msgs ros-kinetic-amcl ros-kinetic-map-server ros-kinetic-move-base ros-kinetic-urdf ros-kinetic-xacro ros-kinetic-compressed-image-transport ros-kinetic-rqt-image-view ros-kinetic-gmapping ros-kinetic-navigation ros-kinetic-interactive-markers ros-kinetic-robot-state-publisher

RUN rosdep update

WORKDIR /root/catkin_ws/src
RUN git clone https://github.com/ROBOTIS-GIT/hls_lfcd_lds_driver.git && \
    git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git && \
    git clone https://github.com/ROBOTIS-GIT/turtlebot3.git

WORKDIR /root/catkin_ws
RUN bash -c "source /opt/ros/kinetic/setup.bash && \
    catkin_make && \
    rosrun create_udev_rules"

COPY docker-entrypoint.sh /root/docker-entrypoint
COPY resin-ip.py /root/resin-ip.py
ENTRYPOINT ["/root/docker-entrypoint"]
CMD ["roscore", "-v", "-p", "11311"]
