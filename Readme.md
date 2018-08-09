# Tugboat

This is code for a self driving warehouse automation vehicle. Well, it will be eventually. It is structured as a http://resin.io application that runs several ROS services on a Raspberry Pi running ResinOS via Resin. The Pi forwards some ROS messages via `rosserial` to an OpenCR daughter board that then drives the motors and outputs odometry information. 

### Contents

 - `core`: Resin docker service for running `roscore`
 - `tugboat`: Resin docker service for running the tugboat ROS nodes to actually do useful stuff
 - `metrics`: Resin docker service for instrumenting the raspberry pi and forwarding telemetry data to the cloud
 - `opencr_firmware`: Arduino sketch running on the OpenCR to listen to commands from the Pi and interface with hardware
 - `devhost`: Not-resin dockerfile and utility files for running a reliable, repeatable, synced ROS install on your desktop 

### Testing the firmware

The `docker-compose.devfirmware.yml` file specifies a set of containers useful for testing the OpenCR firmware. Once a firmware is flashed to an OpenCR connected to your host, run `docker-compose -f docker-compose.devfirmware.yml up` to start a `roscore` and a `rosserial` process that will send around ROS messages for you. To then interact with the system, run a shell with `docker-compose -f docker-compose.devfirmware.yml run shell bash` to get an interactive bash prompt with the right ROS dependencies and environment set up for you. The `opencr_firmware` folder is mounted at `/root/app` inside your shell and the other containers.

To publish an arbitrary message, you can use `rostopic pub` like so:
 - `rostopic pub -r 10 /cmd_vel geometry_msgs/Twist  '{linear:  {x: 0.1, y: 0.0, z: 0.0}, angular: {x: 0.0,y: 0.0,z: 0.0}}'`

To test the motor control systems, you can use the `test/motor_test.py` script that will send varying linear and angular velocities to `/cmd_vel`. Run with `python /root/app/test/motor_test.py`.