#define ARDUINO 100
#define CONTROL_MOTOR_SPEED_FREQUENCY          30   //hz
#define IMU_PUBLISH_FREQUENCY                  200  //hz
#define CMD_VEL_PUBLISH_FREQUENCY              30   //hz
#define DRIVE_INFORMATION_PUBLISH_FREQUENCY    30   //hz
#define VERSION_INFORMATION_PUBLISH_FREQUENCY  1    //hz 
#define DEBUG_LOG_FREQUENCY                    10   //hz 

#define WHEEL_NUM                        2
#define WHEEL_RADIUS                     0.033     // meter
#define WHEEL_SEPARATION                 0.16      // meter (BURGER => 0.16, WAFFLE => 0.287)
#define ROBOT_LENGTH                     0.165     // meter

#define LEFT                             0
#define RIGHT                            1

#define LINEAR                           0
#define ANGULAR                          1