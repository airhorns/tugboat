FROM tugboat_core

# Turtlebot3 ROS dependencies
RUN apt-get install -y ros-kinetic-rviz

COPY docker-entrypoint.sh /root/docker-entrypoint
ENTRYPOINT ["/root/docker-entrypoint"]
