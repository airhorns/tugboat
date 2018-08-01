# Tugboat

This is code for a self driving warehouse automation vehicle. Well, it will be eventually. It is structured as a http://resin.io application that runs several ROS services on a Raspberry Pi running ResinOS via Resin. The Pi forwards some ROS messages via `rosserial` to an OpenCR daughter board that then drives the motors and outputs odometry information. 

### Contents

 - `core`: Resin docker service for running `roscore`
 - `tugboat`: Resin docker service for running the tugboat ROS nodes to actually do useful stuff
 - `metrics`: Resin docker service for instrumenting the raspberry pi and forwarding telemetry data to the cloud
 - `opencr_firmware`: Arduino sketch running on the OpenCR to listen to commands from the Pi and interface with hardware
 - `devhost`: Not-resin dockerfile and utility files for running a reliable, repeatable, synced ROS install on your desktop 

# Bringup

- source /opt/ros/kinetic/setup.bash && export ROS_MASTER_URI=http://core:11311
- roslaunch turtlebot3_bringup turtlebot3_robot.launch

