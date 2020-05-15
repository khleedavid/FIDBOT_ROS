/************************************************************
 * this program controls the MOTORS. 
 * since the subscriber receives 
 * angular VELOCITY (in PWM) and linear VELOCITY (in PWM) as targets,
 * PID control must be relegated to the raspi instead of the arduino.
 * when the subscriber receives the Awheel and Dwheel in PWM,
 * it is sent to the PWM controls of the motors [OUTPUT 0,1]
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

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "PID_v1.h"
#include <TimerOne.h>

//Define Variables we'll be connecting to
//PID vars
#define PID_ENABLED 1 //this is a contingency if the velocity control doesnt work out
double Setpoint[2], Input[2], Output[2];
#ifdef PID_ENABLED
//Define the aggressive and conservative Tuning Parameters
double aggKp=400, aggKi=0.2, aggKd=1;
double consKp=700, consKi=0.2, consKd=0.1;
//Specify the links and initial tuning parameters
PID motorPID[2] = {
	PID(&Input[0], &Output[0], &Setpoint[0], consKp, consKi, consKd,P_ON_M, DIRECT),
  PID(&Input[1], &Output[1], &Setpoint[1], consKp, consKi, consKd,P_ON_M, DIRECT),
	};
#endif
//Encoder Vars
Encoder Enc[2] = {
	Encoder(24,18),
	Encoder(26,19)
	};
long oldPos[2]  = {-999,-999};
//Motor Driver Vars
float testDist = 250; //fixed pwm signal for testing motors and encoder outputs
int motor[4] = {28,30,29,31}; //declare pins 8-11 for motor control. 2 pins >> 1 motor
int EN[2] = {7,6};

//FUNCTION DECLARATIONS
//
//void ROS_init();
//void ROS_update();
void MOTOR_init();
void buttonCheck();
void MOTOR_loop();
void MOTOR_off();
void MOTOR_turn(int motorSel, int turndist);
double ENCODER_update(int encSel);
void PID_init();
void PID_update();

// ================================================================
// ===            ROS_FUNCTIONS                       ===
// ================================================================ 
//ros::NodeHandle  nh;  //create a ros::header class to allow us to create pubs and subs
//
//void messageCb( const std_msgs::Empty& toggle_msg){ 
  ////callback function that returns VOID, takes a const ref of your messagetype as its argument.
  //digitalWrite(13, HIGH-digitalRead(13));   // blink the led
//}
//
//void angleCallback(const std_msgs::Float32& angle_motor){
  ////[subscriber]set angle PWM target
  //Output[1] = angle_motor.data;
//}
//
//void distanceCallback(const std_msgs::Float32& distance_msg){
  ////[subscriber]set distance PWM target
  //Output[0] = distance_msg.data;
//}
//
//ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb ); //create a ros::subscriber class
//ros::Subscriber<std_msgs::Float32> Amotor("Amotor_cmd", angleCallback ); //create a ros::subscriber class
//ros::Subscriber<std_msgs::Float32> Dmotor("Dmotor_cmd", distanceCallback ); //create a ros::subscriber class
//
//
//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);  
////create a ros::publisher class which has topic name "chatter" and messagetype string
//char hello[13] = "hello world!"; //fill a char array with the message to be sent
//
//std_msgs::Int16 Amotor_enc;
//ros::Publisher Amotor_pub("Amotor_pub", &Amotor_enc);  
////create a ros::publisher class which has topic name "Amotor_pub" and messagetype Int16
//
//std_msgs::Int16 Dmotor_enc;
//ros::Publisher Dmotor_pub("Dmotor_pub", &Dmotor_enc);  
////create a ros::publisher class which has topic name "Dmotor_pub" and messagetype Int16
//
//
//char base_link[] = "/base_link";
//char odom[] = "/odom";
//
//void ROS_init(){
  //nh.initNode(); //init node must always come before all other ROS related functions
  ////advertise publishers
  //nh.advertise(chatter);  //advertise the chatter message before it starts publishing
  //nh.advertise(Amotor_pub);  //advertise the Amotor message before it starts publishing
  //nh.advertise(Dmotor_pub);  //advertise the Dmotor message before it starts publishing
  ////subscribe to rostopics
  //nh.subscribe(sub);  //subscribe to the 'sub' topic to receive input
  //nh.subscribe(Amotor);  //subscribe to the 'Amotor' topic to receive input
  //nh.subscribe(Dmotor);  //subscribe to the 'Dmotor' topic to receive input
//}
//
//void ROS_update(){
  //str_msg.data = hello; //attach the hello array with the message to the str_msg string object
  //chatter.publish( &str_msg );  //publish the str_msg string message
//
  ////attach Amotor_enc to message
  //Amotor_enc.data = ENCODER_update(0);
  //Amotor_pub.publish(&Amotor_enc);
  ////attach Dmotor_enc to message
  //Dmotor_enc.data = ENCODER_update(1);
  //Dmotor_pub.publish(&Dmotor_enc);
  //
  //nh.spinOnce();  //handle ROS communication callbacks
//}

// ================================================================
// ===					  MOTOR_FUNCTIONS                       ===
// ================================================================	
void MOTOR_init() {	//set motors as outputs
	for (int i = 0; i < 4; i++) { //init motor pins as output
		pinMode(motor[i], OUTPUT);
	}
	pinMode(LED_BUILTIN, OUTPUT);// initialize LED_BUILTIN as an output.
	pinMode(EN[0], OUTPUT);// initialize EN1 as an output.
	pinMode(EN[1], OUTPUT);// initialize EN2 as an output.
	pinMode(17, INPUT);// initialize PIN17 as an INPUT.
	Serial.println(F("Motors Initialised"));
}
void buttonCheck(int buttonPin) {
	bool buttonState = digitalRead(buttonPin);

// check if the pushbutton is pressed.
// if it is, the buttonState is HIGH:
	if (buttonState) {
		// turn LED on:
		digitalWrite(LED_BUILTIN, HIGH);
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW);
		MOTOR_loop();
	}
}
void MOTOR_loop() { //loops through all 4 motors CW and CCW
	for (int i = 0; i < 2; i++) {
		digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
		MOTOR_turn(i, testDist);
		delay(4000);                       // wait for a second
		digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
		MOTOR_turn(i, -testDist);
		delay(1000);                       // wait for a second
		MOTOR_off();
	}
}
void MOTOR_off() { //turns off all motors
	for (int i = 0; i < 2; i ++) {
		digitalWrite(motor[i], LOW);
		digitalWrite(motor[i+2], LOW);
		analogWrite(EN[i], 0);
	}
}

void MOTOR_turn(int motorSel, int turndist) { //turns selected motors clockwise
	//use turndist to calculate how much to turn motor.
	int i = motorSel;
	if(turndist >=0){	//turn CW
		digitalWrite(motor[i], LOW);
		digitalWrite(motor[i + 2], HIGH);
		analogWrite(EN[i],turndist);
	}
	else{	//turn CCW
		digitalWrite(motor[i], HIGH);
		digitalWrite(motor[i + 2], LOW);
		analogWrite(EN[i],abs(turndist));
	}
}

double ENCODER_update(int encSel){
	double newPos[2] = {0,0};
		newPos[encSel] = Enc[encSel].read();
		if (newPos[encSel] != oldPos[encSel]) {
			oldPos[encSel] = newPos[encSel];}
		Serial.println(newPos[encSel]);// removed to increase speed
		return newPos[encSel];
}


void PID_init(){
	#ifdef PID_ENABLED
	for (int i=0; i<2; i++)
	{
		motorPID[i].SetMode(AUTOMATIC);
		motorPID[i].SetSampleTime(1);
		motorPID[i].SetOutputLimits(100,10,510);
	}	
	Serial.println(F("PID initialised"));
	#endif
}

void PID_update(){
	for (int i=0; i<2; i++)
	{   
    #ifdef PID_ENABLED
		Input[i] = ENCODER_update(i);
		motorPID[i].Compute();
		MOTOR_turn(i,(Output[i]-255));
    if(abs(Input[i] - Setpoint[0]) < 10)MOTOR_off();
     #else     
//      MOTOR_turn(i,Output[i]);
     #endif
	}
}
	
// The setup() function runs once each time the micro-controller starts
void setup()
{	
	MOTOR_init();
  Serial.begin(57600);
	PID_init();
  Setpoint[0]=500;
  Setpoint[1]=500;
	//ROS_init();
}

// Add the main program code into the continuous loop() function
void loop()
{	
//	PID_update(); //update motor outputs
  MOTOR_loop();
	//ROS_update();
	//buttonCheck(17);
}
