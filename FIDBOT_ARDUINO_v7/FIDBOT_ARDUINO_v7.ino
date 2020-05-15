// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       FIDBOT_ARDUINO_v6.ino
    Created:	10/4/20 10:41:03
    Author:     DAVID-ASUS\khlee
*/

// Define User Types below here or use a .h file
//
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
// ===                    FUNCTION PROTOTYPES                ===
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

// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// ================================================================
// ===                TIMER_ROUTINES                       ===
// ================================================================
static void timer_init(){        // initialize timers
	pinMode(interruptPin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPin), buttonCheck, CHANGE);
}

void buttonCheck() {
	// run if the pushbutton is pressed.
	/*ROS_calibrate();*/
}
// ================================================================
// ===            ULTRASONIC SENSOR_FUNCTIONS                       ===
// ================================================================

void ultrasonic_init(){

 sonarBinsArr.layout.dim[0].label = frameidBinsArr;
 sonarBinsArr.layout.dim[0].size = 4;
 sonarBinsArr.layout.dim[0].stride = 1*4;
 sonarBinsArr.layout.data_offset = 0;
 sonarBinsArr.layout.dim_length = 0;
 sonarBinsArr.data_length = 15;
 sonarBinsArr.data = (float *)malloc(sizeof(float)*15);

 sonarBinsArr.layout.dim[0].label = frameidBearing;
 sonarBinsArr.layout.dim[0].size = 4;
 sonarBinsArr.layout.dim[0].stride = 4;
 sonarBinsArr.layout.data_offset = 0;
 sonarBinsArr.layout.dim_length = 0;
 sonarBinsArr.data_length = 15;
 sonarBinsArr.data = (float *)malloc(sizeof(float)*15);
 
	for (int i=0; i<4;i++)
	{	pinMode(trig[i], OUTPUT);
		pinMode(echo[i], INPUT);
	}
	pinMode(temperaturePin, INPUT);
}

void ultrasonic_update(){
 float k = 0;
 for (int j = 0; j<4; j++){
  k = checkRange(trig[j],echo[j]);
   for(int i = 0; i<15; i++){
     sonarBinsArr.data[i] = k;
     sonarBearing.data[i] = bearing[j]+(float)i;
   }
     pub_Bearing.publish(&sonarBearing);
     pub_BinsArr.publish(&sonarBinsArr);
     nh.spinOnce();  //handle ROS communication callbacks
 }
}

float getVoltage(int pin) {
	return (analogRead(pin) * 0.004882814); //Converting from 0 to 1024 to 0 to 5v
}

float microsecondsToMeters(unsigned long microseconds, float temp) {
	return (microseconds * (331.3 + 0.606 * temp)*0.0001) / 2;
	//Multiplying the speed of sound through a certain temperature of air by the
	//length of time it takes to reach the object and back, divided by two
}

int checkRange(uint8_t trigPin, uint8_t echoPin){
	float temperature = (getVoltage(temperaturePin) - 0.5) * 100;
	unsigned long duration; //Set time and cm for distance sensing

	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);

float i = microsecondsToMeters(duration, temperature);
return i;
}
// ================================================================
// ===            MPU9250_FUNCTIONS                       ===
// ================================================================
//
void MPU9250_init(){
	// Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      mpu9250_pub.data = ("Unable to communicate with MPU-9250");
      delay(5000);
    } // while (1)
  } // imu.begin
	
/*--------- SET PARAMETERS FOR IMU ---------*/
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS); // Enable all sensors
  imu.setGyroFSR(2000); // +/- 250, 500, 1000, or 2000 dps
  imu.setAccelFSR(2); // +/- 2, 4, 8, or 16 g
  imu.setLPF(5);  // accel and gyro; 188, 98, 42, 20, 10, 5 Hz
  imu.setSampleRate(10);  // accel and gyro; 4Hz to 1kHz
  imu.setCompassSampleRate(10); // 1-100Hz  
/*--------- SET PARAMETERS FOR DMP/FIFO ---------*/
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // 6-axis (accel/gyro) quaternion calculation
              DMP_FEATURE_SEND_RAW_ACCEL | // send raw accel values to FIFO
              DMP_FEATURE_GYRO_CAL | // enable gyro calibration -> 0 after 8s of no motion
              DMP_FEATURE_SEND_CAL_GYRO, // send calibrated gyro values to FIFO
              10); // Set DMP FIFO rate: 4-200 Hz
  imu.configureFifo(INV_XYZ_GYRO |INV_XYZ_ACCEL);
  
  mpu9250_array.layout.dim[0].label = "MPU9250 RPY Array";
  mpu9250_array.layout.dim[0].size = 3;
  mpu9250_array.layout.dim[0].stride = 1*3;
  mpu9250_array.layout.data_offset = 0;
  mpu9250_array.layout.dim_length = 0;
  mpu9250_array.data_length = 3;
  mpu9250_array.data = (float *)malloc(sizeof(float)*3);
  nh.advertise(MPU9250_array);
  
  double orientation_stddev = -1;
  mpu9250_msg.orientation_covariance[0] = orientation_stddev;
  mpu9250_msg.orientation_covariance[4] = orientation_stddev;
  mpu9250_msg.orientation_covariance[8] = orientation_stddev;
  
  double linear_acceleration_stddev = -1;
  mpu9250_msg.linear_acceleration_covariance[0] = linear_acceleration_stddev;
  mpu9250_msg.linear_acceleration_covariance[4] = linear_acceleration_stddev;
  mpu9250_msg.linear_acceleration_covariance[8] = linear_acceleration_stddev;
  
  double angular_velocity_stddev = -1;
  mpu9250_msg.angular_velocity_covariance[0] = angular_velocity_stddev;
  mpu9250_msg.angular_velocity_covariance[4] = angular_velocity_stddev;
  mpu9250_msg.angular_velocity_covariance[8] = angular_velocity_stddev;
}

void MPU9250_update(){
  digitalWrite(LED_BUILTIN, LOW); 
  //Serial.println("Running MPU9250_update...");         
  if ( imu.fifoAvailable() )  {
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)  {
      imu.computeEulerAngles();
      mpu9250_msg.header.stamp = nh.now();
      mpu9250_msg.header.frame_id = "MPU9250 sensor_msg array";

    if (millis() > 25000 && millis() < 25500){
      mpu9250_pub.data = ("***Initialisation completed.****");
      totalRoll = 0; totalPitch = 0; readIndex = 0;
      for (int thisReading = 0; thisReading < numAverage; thisReading ++) {
        readingsRoll[thisReading] = 0;
        readingsPitch[thisReading] = 0;}
    } // initialization time

//==============================================================
//=====   (1) DRIFT CORRECTION 
//============================================================== 
/*  ROLL: m = -0.0180 deg/min = -0.0003 deg/s = -0.00003 deg/ms
 *  PITCH: m = -0.0224 deg/min = -0.0003733333 deg/s = -0.00003733333 deg/ms
 *  INPUT: RAW ANGLE, (rawRoll, rawPitch); OUTPUT: corrected angle (outRoll, outPitch)
 *  CORRECTION STEP: output - m*time step,t (outRoll, outPitch) time step = time/100
 */ 
float rawRoll = imu.roll;
float rawPitch = imu.pitch;

unsigned long t = (millis()/100); // change per unit time to 100ms
float outRoll = rawRoll - (-0.00003)*t;
float outPitch = rawPitch - (-0.0003733333)*t;

//==============================================================
//=====   (2) ZERO ERROR CORRECTION
//==============================================================  
/*  ROLL: y = 1.0668x - 3.6853 => x = (1/1.0668)*(y + 3.6853)
 *  PITCH: y = 0.9332x + 3.2753 => x = (1/0.9332)*(y - 3.2753)
 *  INPUT: RAW ANGLE, y (rawRoll, rawPitch); OUTPUT: TRUE ANGLE,x (outRoll, outPitch)
 */
outRoll = (1/1.0668)*(outRoll + 3.6853);
outPitch = (1/0.9332)*(outPitch - 3.2753)-0.54;

//==============================================================
//=====   (3) NOISE CORRECTION 
//============================================================== 
/*  MOVING AVERAGE BY 150 UNITS
 */ 
      // moving average of roll and pitch outputs
      totalRoll = totalRoll - readingsRoll[readIndex];
      readingsRoll[readIndex] = outRoll;
      totalRoll = totalRoll + readingsRoll[readIndex];
      
      totalPitch = totalPitch - readingsPitch[readIndex];
      readingsPitch[readIndex] = outPitch;
      totalPitch = totalPitch + readingsPitch[readIndex];

      readIndex = readIndex + 1;
      if (readIndex >= numAverage){
        readIndex = 0; }

      outRoll = totalRoll/numAverage;
      outPitch = totalPitch/numAverage;

	  mpu9250_msg.linear_acceleration.x = imu.calcAccel(imu.ax);
	  mpu9250_msg.linear_acceleration.y = imu.calcAccel(imu.ay);
	  mpu9250_msg.linear_acceleration.z = imu.calcAccel(imu.az);
      
      float gyroX = imu.calcGyro(imu.gx);
      float gyroY = imu.calcGyro(imu.gy);
      float gyroZ = imu.calcGyro(imu.gz);
		mpu9250_msg.angular_velocity.x = gyroX;
		mpu9250_msg.angular_velocity.y = gyroY;
		mpu9250_msg.angular_velocity.z = gyroZ;

      mpu9250_msg.orientation.w = imu.calcQuat(imu.qw);
      mpu9250_msg.orientation.x = imu.calcQuat(imu.qx);
      mpu9250_msg.orientation.y = imu.calcQuat(imu.qy);
      mpu9250_msg.orientation.z = imu.calcQuat(imu.qz);
        
      // output data only when imu is stationary  
	  if (fabs(gyroX)<0.2 && fabs(gyroY)<0.2 && fabs(gyroZ)<0.2){
        mpu9250_array.data[0] = (imu.roll);
        mpu9250_array.data[1] = (imu.pitch);   
      } 
       
    } // update fifo
  } // fifo available
} // MPU9250_update

// ================================================================
// ===					  MOTOR_FUNCTIONS                       ===
// ================================================================
void MOTOR_init() {	//set motors as outputs
	for (int i = 0; i < 4; i++) { //init motor pins as output
		pinMode(motor[i], OUTPUT);
	}
	pinMode(LED_BUILTIN, OUTPUT);// initialize LED_BUILTIN as an output.
	pinMode(EN[0], OUTPUT);// initialize LED_BUILTIN as an output.
	pinMode(EN[1], OUTPUT);// initialize LED_BUILTIN as an output.
}
//void MOTOR_loop() { //loops through all motors CW and CCW
//float testDist = 250; //fixed pwm signal for testing motors and encoder outputs
//	for (int i = 0; i < 2; i++) {
//		MOTOR_turn(i, testDist);
//		delay(1000);					   // wait for a second
//		MOTOR_off();
//		MOTOR_turn(i, -testDist);
//		delay(1000);                       // wait for a second
//		MOTOR_off();
//	}
//}
void MOTOR_off() { //turns off all motors
		digitalWrite(motor[0], LOW);
		digitalWrite(motor[2], LOW);
		analogWrite(EN[0], 0);
    digitalWrite(motor[1], LOW);
    digitalWrite(motor[3], LOW);
    analogWrite(EN[1], 0);
}

void MOTOR_turn(int motorSel, int turndist) { //turns selected motors clockwise
	//use turndist to calculate how much to PWM motor
	int i = motorSel;
	if(turndist >=0){	//turn CW
		digitalWrite(motor[i], HIGH);
		digitalWrite(motor[i + 2], LOW);
		analogWrite(EN[motorSel],abs(turndist));
	}
	else{	//turn CCW
		digitalWrite(motor[i], LOW);
		digitalWrite(motor[i + 2], HIGH);
		analogWrite(EN[motorSel],abs(turndist));
	}
}
// ================================================================
// ===            ROS_FUNCTIONS                       ===
// ================================================================
void ROS_init(){
	nh.getHardware()->setBaud(115200);
	nh.initNode(); //init node must always come before all other ROS related functions
	//advertise publishers
	nh.advertise(Amotor_pub);  //advertise the Amotor message before it starts publishing
	nh.advertise(Dmotor_pub);  //advertise the Dmotor message before it starts publishing
	nh.advertise(MPU9250_pub); //advertise the MPU9250 message before it starts publishing
	nh.advertise(MPU9250_msg);
	nh.advertise(MPU9250_heading);

	//subscribe to ros topics
	nh.subscribe(Amotor);  //subscribe to the 'A motor' topic to receive input
	nh.subscribe(Dmotor);  //subscribe to the 'D motor' topic to receive input
	
	 nh.advertise(pub_Bearing);
	 nh.advertise(pub_BinsArr);
}

void ROS_update(){
//	attach Amotor_enc to message
	Amotor_enc.data = Enc[0].read();
	Amotor_pub.publish(&Amotor_enc);
	nh.spinOnce();  //handle ROS communication callbacks
	
	//attach Dmotor_enc to message
	Dmotor_enc.data = Enc[1].read();
	Dmotor_pub.publish(&Dmotor_enc);
	nh.spinOnce();  //handle ROS communication callbacks

	// attach MPU9250 message
	  MPU9250_update();
	  MPU9250_msg.publish(&mpu9250_msg);          // publish the mpu9250_msg message
	  MPU9250_array.publish(&mpu9250_array);      // publish the mpu9250_array message
	  MPU9250_heading.publish(&mpu9250_heading);  // publish the mpu9250_heading message
	  nh.spinOnce();

	//update ultrasonic sensors
	 ultrasonic_update();
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
	//INIT TIMER
	timer_init();  //initial start time
	MOTOR_init();
	MPU9250_init();
	ultrasonic_init();
	ROS_init();
}

// Add the main program code into the continuous loop() function
void loop()
{
	ROS_update();
}
