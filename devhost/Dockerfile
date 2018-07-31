FROM ros:kinetic-ros-base

# Turtlebot3 ROS dependencies
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y curl wget vim python-rosinstall ros-kinetic-rosserial-python ros-kinetic-tf ros-kinetic-teleop-twist-keyboard ros-kinetic-laser-proc ros-kinetic-rgbd-launch ros-kinetic-depthimage-to-laserscan ros-kinetic-rosserial-arduino ros-kinetic-rosserial-python ros-kinetic-rosserial-server ros-kinetic-rosserial-client ros-kinetic-rosserial-msgs ros-kinetic-amcl ros-kinetic-map-server ros-kinetic-move-base ros-kinetic-urdf ros-kinetic-xacro ros-kinetic-compressed-image-transport ros-kinetic-rqt-image-view ros-kinetic-gmapping ros-kinetic-navigation ros-kinetic-interactive-markers ros-kinetic-robot-state-publisher

# Turtlebot3 Host dependencies
RUN echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list && \
    wget http://packages.osrfoundation.org/gazebo.key -O - | apt-key add - && \
    apt-get update && \
    apt-get install -y ros-kinetic-rviz ros-kinetic-gazebo-ros mesa-utils gazebo7 libgazebo7-dev ros-kinetic-cartographer ros-kinetic-cartographer-ros ros-kinetic-cartographer-ros-msgs ros-kinetic-cartographer-rviz ros-kinetic-frontier-exploration ros-kinetic-navigation-stage

RUN rosdep update

WORKDIR /root/catkin_ws/src
RUN git clone https://github.com/ROBOTIS-GIT/hls_lfcd_lds_driver.git && \
    git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git && \
    git clone https://github.com/ROBOTIS-GIT/turtlebot3.git && \
    git clone https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git

WORKDIR /root/catkin_ws
RUN bash -c "source /opt/ros/kinetic/setup.bash && \
    catkin_make && \
    rosrun create_udev_rules"

COPY docker-entrypoint.sh /root/docker-entrypoint
ENTRYPOINT ["/root/docker-entrypoint"]
CMD ["roscore", "-v", "-p", "11311"]
