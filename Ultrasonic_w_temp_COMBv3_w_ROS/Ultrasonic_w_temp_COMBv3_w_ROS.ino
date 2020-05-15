/*******************************************************************************************************
Improved Ultrasonic Range Sensing Created by Calvin Kielas-Jensen

Using an Arduino UNO, connect a TMP36 temperature sensor data pin to A0 and a 4 pin ultrasonic range sensor with trig pin on digital pin 8 and echo pin on digital pin 9.

This script improves the accuracy of an ultrasonic range sensor by measuring the ambient temperature. 
Sound moves through air at a speed dependant on the ambient temperature according to the following equation: 
Vair = (331.3 + 0.606 * Tc) m/s where Vair is the speed of sound and Tc is the temperature in celsius.

we want each sensor to publish its 
location: which position on the arc of the robot base is the sensor mounted on?
heading: which direction is the sensor pointing? how do we account for overlapping readings?
and distance detected: this has already been written.
this cannot be accomplished using the normal range message code. hence, we need to 
(A) publish a custom message type containing these variables and enable the receiving code to interpret the results
(B) publish individual message types containing consolidated values of location, heading, and distance as numbers to be further combined in the sonarInjector code.
we go with option (B) as (A) requires additional library definitions on both sides to achieve.

step 1: each sensor is equidistant from the center of the robot but has its own (X,Y) coordinates depending on its position along the circumference of the robot.
		[BEARING1,2,3,4]
step 2: to ensure that overlapping readings do not mistake the size of an obstacle, 
		the desired range of the sensor must be limited by the intersection point of the sensor detection sectors.
		[MAX_RANGE]
		this is further limited by the deadzone range created due to the space between each sensor.
step 3: once these parameters are defined, an object which enters a specific sector can be detected with a bearing, approximate width, and distance from robot.
		if an object enters sector at distance x, we assume a line of points have been detected along the arc of a sector with radius x.
		ie. from 0-15 degrees, the publisher will place a point at x distance from the robot for each degree change. is 1 degree fine enough?


Anyone is welcome to use and modify this code as long as I am given credit. Thank you for respecting the open source movement!
*******************************************************************************************************/

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include <std_msgs/String.h>

#define MAX_RANGE  300; //IN m
#define MIN_RANGE  2; //IN m
/**********************
SENSOR SETUP PARAMETERS
***********************/
ros::NodeHandle  nh;

std_msgs::Float32 sonarBearing;
std_msgs::Float32MultiArray sonarBinsArr;


ros::Publisher pub_Bearing("sonarBearing", &sonarBearing);
ros::Publisher pub_BinsArr("sonarBinsArr", &sonarBinsArr);
//
char frameidBearing[] = "/sonarBearing";
char frameidBinsArr[] = "/sonarBinsArr";

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";
//The majority of this code is taken and modified from the ARDX temperature sensor code and the PING sensor code.

int temperaturePin = A0; //Set the temperature pin input 

boolean debug = true; //For serial communication set debug to true, for faster code set debug to false


long cm[4]={0,0,0,0};
int trig[4] = {36,38,40,42}, echo[4] = {37,39,41,43}; //Set pins for trig and echo
float bearing[4] = {30, 45, 60, 75}; //IN RAD

float getVoltage(int pin) { 
  return (analogRead(pin) * 0.004882814); //Converting from 0 to 1024 to 0 to 5v 
}

float microsecondsToMeters(long microseconds, float temp) { 
  return (microseconds * (331.3 + 0.606 * temp)*0.0001) / 2; 
  //Multiplying the speed of sound through a certain temperature of air by the 
  //length of time it takes to reach the object and back, divided by two 
}

float checkRange(int trigPin, int echoPin){
  float temperature = (getVoltage(temperaturePin) - 0.5) * 100;
  long duration; //Set time and cm for distance sensing
  
//  if (debug) { Serial.println(temperature); }
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse: 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH);
  
  float i = microsecondsToMeters(duration, temperature); 
  if (i>300){i=MAX_RANGE;}
  if (i<2){i=MIN_RANGE;}
  return i;
}

void setup() { 
  //nh.getHardware()->setBaud(57600);
  nh.initNode();
  
  nh.advertise(chatter);
  nh.advertise(pub_Bearing);
  
  sonarBinsArr.layout.dim[0].label = frameidBinsArr;
  sonarBinsArr.layout.dim[0].size = 3;
  sonarBinsArr.layout.dim[0].stride = 1*3;
  sonarBinsArr.layout.data_offset = 0;
  sonarBinsArr.layout.dim_length = 0;
  sonarBinsArr.data_length = 4;
  sonarBinsArr.data = (float *)malloc(sizeof(float)*3);
  nh.advertise(pub_BinsArr);

  Serial.begin(57600);
  for (int i=0;i<4  ;i++){
  pinMode(trig[i], OUTPUT); 
  pinMode(echo[i], INPUT);
  }
  pinMode(temperaturePin, INPUT);
}

long range_time;
//we need the sensors to check in sequence and publish both bearing and distance
void loop(){ 
     float serialSonarArr[15] = {0};
     float serialBearing = 0;
    //publish an array of points for each sensor covering the range of bearings at regular intervals
    //there are 4 sensors, covering a total of 60 degrees on the front of the robot
    //when the first sensor publishes, it reads the distance from the obstacle as an arc.
    //this arc is represented by individual increments of the bearing, as the point is "drawn" into an arc, resulting in a 2d pointcloud
    //the sensor readings transitions into the subsequent sensors incrementally until the 60 degrees arc is complete and the code loops
    //we set this up as such:
    //loop for each of the 4 sensors
    //within each sensor, loop for each degree, making 15 publishes per sensor. publish every time each degree is measured.
    //when publishing each reading, include the current bearing. increment bearing by 1 degree each time the sensor is published.
    for (int j = 0; j<4; j++){
  	  for(int i = 0; i<16; i++){
          sonarBinsArr.data[i]=checkRange(trig[j],echo[j]);
//          serialSonarArr[i]=checkRange(trig[j],echo[j]);
          sonarBearing.data = bearing[j]+(float)i;
//          serialBearing = bearing[j]+(float)i;
//          
          pub_Bearing.publish(&sonarBearing);
          pub_BinsArr.publish(&sonarBinsArr);
//          Serial.print(serialSonarArr[i]);
//          Serial.print(";");
//          Serial.print(serialBearing);
//          Serial.print("))");
  	  }
//      Serial.print("Count:");
//      Serial.println(j);
      str_msg.data = hello;
      chatter.publish( &str_msg );
    }
  nh.spinOnce();
  delay(100);
}
