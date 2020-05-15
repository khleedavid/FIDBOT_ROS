#include <ros.h>  //include the ROSSERIAL library, as well as the messagetypes that will be used
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>

#include <sensor_msgs/Imu.h>
#include <MPU9250_RegisterMap.h>
#include <SparkFunMPU9250-DMP.h>

#include <Encoder.h>
// ================================================================
// ===                    FUNCTION PROTOTYPES		            ===
// ================================================================
void ROS_init();
void ROS_update();
static void timer_init();
void buttonCheck();
void ultrasonic_init();
void ultrasonic_update();
static float getVoltage(int pin);
float microsecondsToMeters(long microseconds, long temp);
int checkRange(uint8_t trigPin, uint8_t echoPin);
void MPU9250_init();
void MPU9250_update();
void MPU9250_mag();
void MOTOR_init();
void MOTOR_loop();
void MOTOR_off();
void MOTOR_turn(int motorSel, int turndist);

// ================================================================
// ===            ULTRASONIC SENSOR_VARIABLES                   ===
// ================================================================
float bearing[4] = {48*DEG_TO_RAD, 63*DEG_TO_RAD, 78*DEG_TO_RAD, 93*DEG_TO_RAD}; //IN RAD
const uint8_t temperaturePin = A0; //Set the temperature pin input
uint8_t echo[4] = {36,38,40,42}, trig[4] = {37,39,41,43}; //Set pins for trig and echo
// ================================================================
// ===                    MPU9250 VARIABLES                     ===
// ================================================================
MPU9250_DMP imu;

const uint8_t numAverage = 100;
const uint8_t interval = 100;
int readIndex = 0;
float totalRoll = 0;
float totalPitch = 0;
float offsetRoll = 0;
float offsetPitch = 0;
float readingsRoll[numAverage];
float readingsPitch[numAverage];
float initialHeading = 0;
// ================================================================
// ===             PID, MOTOR VARIABLES              ===
// ================================================================
long Input[2], Output[2]= {0,0};

Encoder Enc[2] = {
	Encoder(18,24),
	Encoder(26,19)
};
long oldPos[2]  = {-999,-999};
//Motor Driver Vars
static const uint8_t motor[4] = {28,31,29,30}; //declare pins 8-11 for motor control. 2 pins >> 1 motor
static const uint8_t EN[2] = {7,6}; //[1] is angle, [0] is distance
	
// ================================================================
// ===             ROS VARIABLES              ===
// ================================================================

ros::NodeHandle  nh;  //create a ros::header class to allow us to create pubs and subs

void angleCallback(const std_msgs::Float32& angle_motor){
	//[subscriber]set angle PWM target
	MOTOR_turn(1,angle_motor.data);
}

void distanceCallback(const std_msgs::Float32& distance_msg){
	//[subscriber]set distance PWM target
	MOTOR_turn(0,distance_msg.data);
}
void messageCb( const std_msgs::Empty& toggle_msg){
	buttonCheck(); //run buttonCheck when command is sent
}
ros::Subscriber<std_msgs::Empty> button_toggle("toggle_button", &messageCb ); //create a ros::subscriber class
ros::Subscriber<std_msgs::Float32> Amotor("Amotor_cmd", angleCallback ); //create a ros::subscriber class
ros::Subscriber<std_msgs::Float32> Dmotor("Dmotor_cmd", distanceCallback ); //create a ros::subscriber class

std_msgs::Int32 Amotor_enc;
ros::Publisher Amotor_pub("Amotor_pub", &Amotor_enc);
//create a ros::publisher class which has topic name "Amotor_pub" and messagetype Int32

std_msgs::Int32 Dmotor_enc;
ros::Publisher Dmotor_pub("Dmotor_pub", &Dmotor_enc);
//create a ros::publisher class which has topic name "Dmotor_pub" and messagetype Int32

std_msgs::String mpu9250_pub; // create publishing variables
ros::Publisher MPU9250_pub("MPU9250_pub",&mpu9250_pub);
//create a ros::publisher class which has topic name "MPU9250_pub" and messagetype string

std_msgs::Float64MultiArray mpu9250_array;
ros::Publisher MPU9250_array("MPU9250_array",&mpu9250_array);
//create a ros::publisher class which has topic name "MPU9250_array" and messagetype Float64MultiArray

sensor_msgs::Imu mpu9250_msg;
ros::Publisher MPU9250_msg("imu",&mpu9250_msg);
//create a ros::publisher class which has topic name "MPU9250_msg" and messagetype sensor_msg

std_msgs::Float64 mpu9250_heading;
ros::Publisher MPU9250_heading("MPU9250_heading",&mpu9250_heading);
//create a ros::publisher class which has topic name "MPU9250_heading" and messagetype string

std_msgs::Float32MultiArray sonarBearing;
std_msgs::Float32MultiArray sonarBinsArr;

ros::Publisher pub_Bearing("sonarBearing", &sonarBearing);
ros::Publisher pub_BinsArr("sonarBinsArr", &sonarBinsArr);

static const char frameid[] = "/imu_MPU9250";
static const char base_link[] = "/base_link";
static const char odom[] = "/odom";
static const char frameidBearing[] = "/sonarBearing";
static const char frameidBinsArr[] = "/sonarBinsArr";

// ================================================================
// ===             TIMER VARIABLES                              ===
// ================================================================
static const uint8_t interruptPin =17;
// ================================================================
// ===             ULTRASONIC VARIABLES                         ===
// ================================================================
#define MAX_RANGE  300; //IN cm
#define MIN_RANGE  2; //IN cm
