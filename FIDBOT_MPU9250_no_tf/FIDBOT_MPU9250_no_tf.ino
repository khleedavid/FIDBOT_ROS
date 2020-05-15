/************************************************************
MPU9250_DMP_Quaternion
 Quaternion example for MPU-9250 DMP Arduino Library 
Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library
The MPU-9250's digital motion processor (DMP) can calculate
four unit quaternions, which can be used to represent the
rotation of an object.
This exmaple demonstrates how to configure the DMP to 
calculate quaternions, and prints them out to the Serial
monitor. It also calculates pitch, roll, and yaw from those
values.
Development environment specifics:
Arduino IDE 1.6.12
SparkFun 9DoF Razor IMU M0
Supported Platforms:
- ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
/* Run the rosserial client application 
 * rosrun rosserial_python serial_node.py /dev/ttyACM0
 * rosrun rosserial_arduino serial_node.py _port:=/dev/ttyACM0
 */ 

#include <ros.h>  //include the ROSSERIAL library, as well as the messagetypes that will be used
#include <std_msgs/String.h> //each messagetype is actually a library with a set of allowable variables in a class
#include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <ros/time.h>
#include <ros/duration.h>
#include <sensor_msgs/Imu.h>

#include <SparkFunMPU9250-DMP.h>
#include <Encoder.h>
#include "PID_v1.h"
#include <TimerOne.h>

//MPU9250 Variables
MPU9250_DMP imu;
// ================================================================
// ===             TIMER VARIABLES                ===
// ================================================================
uint32_t globalTimer = 0;
unsigned long currentMicros = 0;
uint32_t blinkTimer, waitTimer = 0;
const unsigned long calibrate_period = 10000000;  //calibration wait

const unsigned long blink_period = 1000;  //blink timer
bool blinkState = false;

static void timer_init();

// ================================================================
// ===            ROS_FUNCTIONS                       ===
// ================================================================ 
ros::NodeHandle  nh;  //create a ros::header class to allow us to create pubs and subs

void messageCb( const std_msgs::Empty& toggle_msg){ 
  //callback fuction that returns VOID, takes a const ref of your messagetype as its argument.
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);  
//create a ros::publisher class which has topic name "chatter" and messagetype string
char hello[13] = "hello world!"; //fill a char array with the message to be sent

sensor_msgs::Imu mpu9250_msg;
ros::Publisher MPU9250_pub("MPU9250_pub", &mpu9250_msg);
//create a ros::publisher class which has topic name "MPU9250_pub" and messagetype Imu

char frameid[] = "/imu_MPU9250";
char base_link[] = "/base_link";
char odom[] = "/odom";


void ROS_init(){
  nh.initNode(); //init node must always come before all other ROS related functions
  //advertise publishers
  nh.advertise(chatter);  //advertise the chatter message before it starts publishing
  nh.advertise(MPU9250_pub); //advertise the MPU9250 message before it starts publishing
}

void ROS_update(){
  str_msg.data = hello; //attach the hello array with the message to the str_msg string object
  chatter.publish( &str_msg );  //publish the str_msg string message

  //attach MPU9250 readings to message
  MPU9250_update();   
  MPU9250_pub.publish( &mpu9250_msg );
  
  nh.spinOnce();  //handle ROS communication callbacks
}
// ================================================================
// ===                TIMER_ROUTINES                       ===
// ================================================================
static void timer_init(){        // initialize timers
  globalTimer = micros();
  blinkTimer = micros();
  waitTimer = micros();
  Timer1.initialize(100000);
}
static void wait(unsigned long wait_time){ //waits a set amount of time in microseconds
  waitTimer = currentMicros;  //reset globaltimer
  currentMicros = micros();
  while (currentMicros - waitTimer <= wait_time) {
    currentMicros = micros();
  }
}
static void blinkLED(){  //blink LED to indicate activity
  currentMicros = micros();  //get the current "time" (actually the number of microseconds since the program started)
  if (currentMicros - blinkTimer >= blink_period) { //test whether the period has elapsed
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);
    blinkTimer = currentMicros;  //IMPORTANT to save the start time of the current LED state.
  }
}

// ================================================================
// ===            MPU9250_FUNCTIONS                       ===
// ================================================================ 
void MPU9250_init(){
  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
//      Serial.println("Unable to communicate with MPU-9250");
//      Serial.println("Check connections, and try again.");
//      Serial.println();
      nh.loginfo("Unable to communicate with MPU-9250");
      delay(5000);
    }
  }
  
  imu.setGyroFSR(2000); // +/- 250, 500, 1000, or 2000 dps
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              100); // Set DMP FIFO rate to 10 Hz
  int a=imu.getAccelFSR();
//  Serial.print("Accel Config: "); Serial.println(a);
//  Serial.println("Roll, Pitch, Yaw, ax, ay, az");
  
  double orientation_stddev;
  mpu9250_msg.orientation_covariance[0] = orientation_stddev;
  mpu9250_msg.orientation_covariance[4] = orientation_stddev;
  mpu9250_msg.orientation_covariance[8] = orientation_stddev;
  
  double linear_acceleration_stddev;
  mpu9250_msg.linear_acceleration_covariance[0] = linear_acceleration_stddev;
  mpu9250_msg.linear_acceleration_covariance[4] = linear_acceleration_stddev;
  mpu9250_msg.linear_acceleration_covariance[8] = linear_acceleration_stddev;
  
  double angular_velocity_stddev;
  mpu9250_msg.angular_velocity_covariance[0] = angular_velocity_stddev;
  mpu9250_msg.angular_velocity_covariance[4] = angular_velocity_stddev;
  mpu9250_msg.angular_velocity_covariance[8] = angular_velocity_stddev;
  
}

void MPU9250_update(){
   // Check for new data in the FIFO
  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      imu.computeEulerAngles();
      // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
      // are all updated.
      // Quaternion values are, by default, stored in Q30 long
      // format. calcQuat turns them into a float between -1 and 1
      mpu9250_msg.header.stamp = nh.now();
      mpu9250_msg.header.frame_id = frameid;

      float accelX = imu.calcAccel(imu.ax);
      float accelY = imu.calcAccel(imu.ay);
      float accelZ = imu.calcAccel(imu.az);

      mpu9250_msg.linear_acceleration.x = accelX;
      mpu9250_msg.linear_acceleration.y = accelY;
      mpu9250_msg.linear_acceleration.z = accelZ;

      float gyroX = imu.calcGyro(imu.gx);
      float gyroY = imu.calcGyro(imu.gy);
      float gyroZ = imu.calcGyro(imu.gz);
      
      mpu9250_msg.angular_velocity.x = gyroX;
      mpu9250_msg.angular_velocity.y = gyroY;
      mpu9250_msg.angular_velocity.z = gyroZ;

      float quatw = imu.calcQuat(imu.qw);
      float quatx = imu.calcQuat(imu.qx);
      float quaty = imu.calcQuat(imu.qy);
      float quatz = imu.calcQuat(imu.qz);
      
      mpu9250_msg.orientation.w = quatw;
      mpu9250_msg.orientation.x = quatx;
      mpu9250_msg.orientation.y = quaty;
      mpu9250_msg.orientation.z = quatz;
  
     }
  }  
}
// The setup() function runs once each time the micro-controller starts
void setup()
{	
	//INIT TIMER
  timer_init();
  MPU9250_init();
	//Serial.begin(57600);
	ROS_init();
}

// Add the main program code into the continuous loop() function
void loop()
{	
//	buttonCheck();
	blinkLED();
  ROS_update();
  delay(1);
}
