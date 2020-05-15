/*******************************************************************************************************
Improved Ultrasonic Range Sensing Created by Calvin Kielas-Jensen

Using an Arduino UNO, connect a TMP36 temperature sensor data pin to A0 and a 4 pin ultrasonic range sensor with trig pin on digital pin 8 and echo pin on digital pin 9.

This script improves the accuracy of an ultrasonic range sensor by measuring the ambient temperature. 
Sound moves through air at a speed dependant on the ambient temperature according to the following equation: 
Vair = (331.3 + 0.606 * Tc) m/s where Vair is the speed of sound and Tc is the temperature in celcius.

The recorded sensor data is passed through ROSSERIAL via the /ultrasoundN topic with the SENSOR_MSGS::RANGE type.

Anyone is welcome to use and modify this code as long as I am given credit. Thank you for respecting the open source movement!
*******************************************************************************************************/

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

#define MAX_RANGE  3; //IN m
#define MIN_RANGE  0.02; //IN m

ros::NodeHandle  nh;

sensor_msgs::Range range_msgN;

ros::Publisher pub_rangeN( "/ultrasoundN", &range_msgN);

char frameidN[] = "/ultrasoundN";

//The majority of this code is taken and modified from the ARDX temperature sensor code and the PING sensor code.

int temperaturePin = A0; //Set the temperature pin input 

boolean debug = true; //For serial communication set debug to true, for faster code set debug to false


int trig = 8, echo = 9; //Set pins for trig and echo

float getVoltage(int pin) { 
  return (analogRead(pin) * 0.004882814); //Converting from 0 to 1024 to 0 to 5v 
}

float microsecondsToMeters(long microseconds, float temp) { 
  return (microseconds * (331.3 + 0.606 * temp)*0.000001) / 2; 
  //Multiplying the speed of sound through a certain temperature of air by the 
  //length of time it takes to reach the object and back, divided by two 
}

float checkRange(int trigPin, int echoPin){
  float temperature = (getVoltage(temperaturePin) - 0.5) * 100;
  long duration; //Set time and cm for distance sensing

  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse: 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH);
//  Serial.print("temperature: ");
//  Serial.println(temperature);
  
	float i = microsecondsToMeters(duration, temperature);
//	if (i>3){i=MAX_RANGE;}
//	if (i<0.02){i=MIN_RANGE;}
	return i; 
//  return microsecondsToMeters(duration, temperature); 
}

void setup() { 
//  nh.initNode();
//  nh.advertise(pub_rangeN);
//  
//  range_msgN.radiation_type = sensor_msgs::Range::ULTRASOUND;
//  range_msgN.header.frame_id =  frameidN;
//  range_msgN.field_of_view = 0.26;  // 15deg = 0.2617rad
//  range_msgN.min_range = 0.02;
//  range_msgN.max_range = 3.0;
  

  if (debug) { Serial.begin(9600); }
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  pinMode(temperaturePin, INPUT);
}

long range_time;

void loop(){ 
  
  if ( millis() >= range_time ){ 
    //publish
//    range_msgN.range = checkRange(trig,echo);
//    range_msgN.header.stamp = nh.now();
//    pub_rangeN.publish(&range_msgN);

    Serial.print("Range:");
    Serial.println(checkRange(trig,echo));
    range_time =  millis() + 50;
  }

//  nh.spinOnce();
}
