// Define User Types below here or use a .h file
//ROS related libraries
#include <ros.h>
#include <ros/time.h>
#include <ros/Duration.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Temperature.h>
#include <tf/transform_broadcaster.h>

//ARDUINO component libraries
#include <Wire.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter
#include <MPU6050_6Axis_MotionApps20.h>
#include <I2Cdev.h>
#include <TimerOne.h>


//DEFINE THESE PARAMETERS BEFORE BEGINNING TESTING
///////////////////////////////////   MPU CONFIGURATION   /////////////////////////////
//Change this 3 variables if you want to fine tune the sketch to your needs.
int buffersize=1000;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone=10;     //Accelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone=2;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)
const float calibration_factor[4] = {123000,66300,102000,76000}; //set calibration factor for load cells

// ================================================================
// ===             MPU  VARIABLES                ===
// ================================================================
//gyroscope MPU6050 datasheet https://components101.com/sensors/mpu6050-module
/* IMU Data */
// Variables to store the values from the sensor readings
#define RESTRICT_PITCH // Comment out to restrict roll to ?90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define INTERRUPT_PIN 2 //dmpDataReady interrupt notifies when data is ready to be read
Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
//mpu config vars
MPU6050 mpu;
bool mpu_isinit = false;
//kalman filter measurement variables
double accX, accY, accZ = 0;
double gyroX, gyroY, gyroZ = 0;
double gyroXangle, gyroYangle, gyroZangle = 0; // Angle calculate using the gyro only
double kalAngleX, kalAngleY = 0; // Calculated angle using a Kalman filter
double roll, pitch = 0;
//calibration variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t temp;
int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,caliState=0;
int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;

// ================================================================
// ===             i2c  VARIABLES                ===
// ================================================================

uint32_t globalTimer = 0;
uint8_t i2cData[14]; // Buffer for I2C data
const uint8_t IMUAddress = 0x68; // AD0 is logic low on the PCB
const uint16_t I2C_TIMEOUT = 1000; // Used to check for errors in I2C communication

// ================================================================
// ===             TIMER VARIABLES                ===
// ================================================================

unsigned long currentMicros = 0;
uint32_t blinkTimer, waitTimer = 0;
const unsigned long calibrate_period = 10000000;  //calibration wait
const unsigned long blink_period = 100000;  //blink timer
bool blinkState = false;

// Define Function Prototypes that use User Types below here or use a .h file
//
static void MPUready();
static void wait(unsigned long wait_time);
static void blinkLED();
static void MPU_calibrate();
static void meansensors();
static void calibration();
static uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop);
static uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop);
static uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes);
static void timer_init();
static Quaternion ToQuaternion(double yaw, double pitch, double roll);

static void MPU_init();
static void MPU_update();
static void MPU_turnCalc(double x, double y, int* dx, int* dy);
static void MPU_settlewait(double pastKalAngleX, double pastKalAngleY);
static void MPU_print();

// Define Functions below here or use other .ino or cpp files
//
// ================================================================
// ===               ROS INITIALISATION			                ===
// ================================================================
Quaternion q;
char frameid[] = "/imu_MPU6050";
char base_link[] = "/base_link";
char odom[] = "/odom";

ros::NodeHandle  nh; // create node handle for rosserial
sensor_msgs::Imu rosimu_msg;
sensor_msgs::Temperature rosimutemp_msg;
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

ros::Publisher imu_pub("rosimu_data", &rosimu_msg);
ros::Publisher imu_temperature_pub("rosimutemp_data", &rosimutemp_msg);
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool stopInterrupt = false; //Emergency stop interrupt
volatile bool MPUcallback = false;
volatile bool LCcallback = false;
static void MPUready(){ //MPU Interrupt Service Routine
	MPUcallback = true;
}

// ================================================================
// ===                TIMER_ROUTINES                       ===
// ================================================================
static void timer_init(){        // initialize timers
	globalTimer = micros();
	blinkTimer = micros();
	waitTimer = micros();
	Timer1.initialize(100000);
	Timer1.attachInterrupt(MPUready);  // attaches MPUready() as a timer overflow interrupt
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
// ===                CALIBRATION_ROUTINE                       ===
// ================================================================
static void MPU_calibrate(){
	MPU_settlewait(kalAngleX,kalAngleY);	//wait for unit to stop swinging
	digitalWrite(LED_BUILTIN,HIGH);
	// 	Serial.print("{MPU6050 Calibration Sketch}");	//print message to notify user
	wait(20000);
	// 	Serial.println(F("{\nYour MPU6050 should be placed in horizontal position, with package letters facing up. \nDon't touch it until you see a finish message.\n}"));
	wait(30000);
	// verify offboard connection to mpu
	Serial.print(mpu.testConnection() ? "{MPU6050 connection successful}" : "{MPU6050 connection failed}");
	while(mpu.testConnection() == false);	//if connection is not made, wait until user solves the issue
	wait(10000);
	// reset mpu raw measurement offsets
	mpu.setXAccelOffset(0);
	mpu.setYAccelOffset(0);
	mpu.setZAccelOffset(0);
	mpu.setXGyroOffset(0);
	mpu.setYGyroOffset(0);
	mpu.setZGyroOffset(0);
	do{
		if (caliState==0 && stopInterrupt!= true){
			// 			Serial.print(F("{Reading sensors...}"));
			meansensors();	//find mean of raw sensor readings for first pass of offset
			caliState++;	//increment calibration to next stage
			wait(100000);
		}
		if (caliState==1 && stopInterrupt!= true) {
			// 			Serial.print(F("{Calculating offsets...}"));
			calibration();	//set offsets to mean values, measure mean of sensor readings again, until sensor mean readings are within desired range of the predefined optimal level measurements
			caliState++;	//increment calibration to next stage
			wait(100000);
		}
		if (caliState==2 && stopInterrupt!= true) {
			meansensors();	//calculate mean of sensors again for final pass
			// set new offsets for sensor readings
			mpu.setXAccelOffset(ax_offset);
			mpu.setYAccelOffset(ay_offset);
			mpu.setZAccelOffset(az_offset);
			mpu.setXGyroOffset(gx_offset);
			mpu.setYGyroOffset(gy_offset);
			mpu.setZGyroOffset(gz_offset);
			wait(100000);
			caliState++;    //increment calibration to next stage
		}
	} while (caliState < 3 && stopInterrupt!= true);
	Serial.print(F("<CALIFIN>"));	//send <CALIFIN> message to end calibration on Main board

	caliState = 0; //reset calibration stages for next calibrate routine
	if (stopInterrupt == true){
		//if stopInterrupt is pressed, calibration process will exit
		//send <R> to confirm that system has exited calibration
		//reset stopInterrupt flag
		Serial.print("<R>");
		stopInterrupt = false;
	}
}
static void meansensors(){
	long i=0,buff_ax=0,buff_ay=0,buff_az=0,buff_gx=0,buff_gy=0,buff_gz=0;

	while (i<(buffersize+101)){
		// read raw accel/gyro measurements from device
		mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		
		if (i>100 && i<=(buffersize+100)){ //First 100 measures are discarded
			buff_ax=buff_ax+ax;
			buff_ay=buff_ay+ay;
			buff_az=buff_az+az;
			buff_gx=buff_gx+gx;
			buff_gy=buff_gy+gy;
			buff_gz=buff_gz+gz;
		}
		if (i==(buffersize+100)){
			mean_ax=buff_ax/buffersize;
			mean_ay=buff_ay/buffersize;
			mean_az=buff_az/buffersize;
			mean_gx=buff_gx/buffersize;
			mean_gy=buff_gy/buffersize;
			mean_gz=buff_gz/buffersize;
		}
		i++;
		delay(2); //Needed so we don't get repeated measures
	}
}
static void calibration(){ //sets offsets by averaging test offset values within a shrinking range
	ax_offset=-mean_ax/8;
	ay_offset=-mean_ay/8;
	az_offset=(16384-mean_az)/8;

	gx_offset=-mean_gx/4;
	gy_offset=-mean_gy/4;
	gz_offset=-mean_gz/4;
	while (1){
		int ready=0;
		mpu.setXAccelOffset(ax_offset);
		mpu.setYAccelOffset(ay_offset);
		mpu.setZAccelOffset(az_offset);

		mpu.setXGyroOffset(gx_offset);
		mpu.setYGyroOffset(gy_offset);
		mpu.setZGyroOffset(gz_offset);

		meansensors();
		// 		Serial.println("{...}");

		if (abs(mean_ax)<=acel_deadzone) ready++;
		else ax_offset=ax_offset-mean_ax/acel_deadzone;

		if (abs(mean_ay)<=acel_deadzone) ready++;
		else ay_offset=ay_offset-mean_ay/acel_deadzone;

		if (abs(16384-mean_az)<=acel_deadzone) ready++;
		else az_offset=az_offset+(16384-mean_az)/acel_deadzone;

		if (abs(mean_gx)<=giro_deadzone) ready++;
		else gx_offset=gx_offset-mean_gx/(giro_deadzone+1);

		if (abs(mean_gy)<=giro_deadzone) ready++;
		else gy_offset=gy_offset-mean_gy/(giro_deadzone+1);

		if (abs(mean_gz)<=giro_deadzone) ready++;
		else gz_offset=gz_offset-mean_gz/(giro_deadzone+1);

		if (ready==6 || stopInterrupt == true) break;
	}
}
// ================================================================
// ===                I2C_PROTOCOLS                          ===
// ================================================================
static uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop)
{
	return i2cWrite(registerAddress, &data, 1, sendStop); // Returns 0 on success
}
static uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop)
{
	Wire.beginTransmission(IMUAddress);
	Wire.write(registerAddress);
	Wire.write(data, length);
	uint8_t rcode = Wire.endTransmission(sendStop); // Returns 0 on success
	if (rcode) {
		Serial.print(F("{i2cWrite failed: "));
			Serial.print(rcode);
		Serial.println("}");
	}
	return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
}
static uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes)
{
	uint32_t timeOutTimer;
	Wire.beginTransmission(IMUAddress);
	Wire.write(registerAddress);
	uint8_t rcode = Wire.endTransmission(false); // Don't release the bus
	if (rcode) {
		Serial.print(F("{i2cRead failed: "));
			Serial.print(rcode);
		Serial.println("}");
		return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
	}
	Wire.requestFrom(IMUAddress, nbytes, (uint8_t)true); // Send a repeated start and then release the bus after reading
	for (uint8_t i = 0; i < nbytes; i++) {
		if (Wire.available())
		data[i] = Wire.read();
		else {
			timeOutTimer = micros();
			while (((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available());
			if (Wire.available())
			data[i] = Wire.read();
			else {
				Serial.println(F("{i2cRead timeout}"));
				return 5; // This error value is not already taken by endTransmission
			}
		}
	}
	return 0; // Success
}
// ================================================================
// ===                MPU_PROTOCOLS                          ===
// ================================================================
static bool SENSORS_testinit(){	//returns true if mpu is initialised
	if (mpu_isinit == true)
	{
		return true;
	}
	else{	 return false;}
}
static void MPU_init()
{
	pinMode(INTERRUPT_PIN, INPUT);
	Wire.begin();
	#if ARDUINO >= 157
	Wire.setClock(400000UL); // Set I2C frequency to 400kHz
	#else
	TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
	#endif

	i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
	i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
	i2cData[2] = 0x00; // Set Gyro Full Scale Range to ?250deg/s
	i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ?2g
	
	while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
	while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode
	
	while (i2cRead(0x75, i2cData, 1));
	if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
		// 		Serial.print(F("{Error reading sensor}"));
		while (1);
	}
	wait(10000);

	/* Set kalman and gyro starting angle */
	while (i2cRead(0x3B, i2cData, 6));
	accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
	accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
	accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
	
	// Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
	// atan2 outputs the value of -? to ? (radians) - see http://en.wikipedia.org/wiki/Atan2
	// It is then converted from radians to degrees
	#ifdef RESTRICT_PITCH // Eq. 25 and 26
	roll  = atan2(accY, accZ) * RAD_TO_DEG;
	pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
	#else // Eq. 28 and 29
	roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
	pitch = atan2(-accX, accZ) * RAD_TO_DEG;
	#endif

	kalmanX.setAngle(roll); // Set starting angle
	kalmanY.setAngle(pitch);
	gyroXangle = roll;
	gyroYangle = pitch;
	mpu_isinit = true; //set mpu initialised flag for mainboard query
}
static void MPU_update(){
	if (!MPUcallback) return;	//check if MPUCallback Flag is true for timing purposes
	MPUcallback = false;	//reset MPUCallback flag
	while (i2cRead(0x3B, i2cData, 14));	//read mpu6050 through i2c register 0x3B
	accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
	accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
	accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
	temp = (int16_t)((i2cData[6] << 8) | i2cData[7]);
	gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
	gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
	gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);
	double dt = (double)(micros() - globalTimer) / 1000000; // Calculate delta time, change in time since last reading
	globalTimer = micros();	//update current time

	// Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
	// atan2 outputs the value of -? to ? (radians) - see http://en.wikipedia.org/wiki/Atan2
	// It is then converted from radians to degrees
	#ifdef RESTRICT_PITCH // Eq. 25 and 26
	// convert accelerometer roll and pitch from radians to degrees
	roll  = atan2(accY, accZ) * RAD_TO_DEG;
	pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
	#else // Eq. 28 and 29
	roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
	pitch = atan2(-accX, accZ) * RAD_TO_DEG;
	#endif
	// convert gyroscope roll and pitch rotation from radians/second to degrees/second
	double gyroXrate = gyroX / 131.0; // Convert to deg/s
	double gyroYrate = gyroY / 131.0; // Convert to deg/s
	double gyroZrate = gyroZ / 131.0; // Convert to deg/s

	#ifdef RESTRICT_PITCH
	//if raw roll reading goes beyond 90 degrees for each side, reset kalman filter angles to prevent roll value from "jumping" between high and low readings
	// This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
	if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
		kalmanX.setAngle(roll);
		kalAngleX = roll;
		gyroXangle = roll;
	} else
	kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the roll angle using a Kalman filter

	if (abs(kalAngleX) > 90)	//if calculated roll reading exceeds 90 degree limit, invert the gyroscope reading
	gyroYrate = -gyroYrate; // Invert rate, so it fits the restricted accelerometer reading
	kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);	// Calculate the pitch angle using a Kalman filter
	#else
	// This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
	if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
		kalmanY.setAngle(pitch);
		kalAngleY = pitch;
		gyroYangle = pitch;
	} else
	kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

	if (abs(kalAngleY) > 90)
	gyroXrate = -gyroXrate; // Invert rate, so it fits the restricted accelerometer reading
	kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
	#endif

	gyroXangle += gyroXrate * dt; // Calculate raw gyro angle without any filter
	gyroYangle += gyroYrate * dt;
	gyroZangle += gyroZrate * dt;

	//get kalman filtered quarternions via conversion
	q = ToQuaternion(gyroZangle,kalAngleY,kalAngleX);
	rosimu_msg.header.stamp = nh.now();
	rosimu_msg.header.frame_id = frameid;
	
	rosimu_msg.angular_velocity.x = gyroXrate;
	rosimu_msg.angular_velocity.y = gyroYrate;
	rosimu_msg.angular_velocity.z = gyroZrate;
	
	rosimu_msg.linear_acceleration.x = accX * (8.0 / 65536.0) * 9.81;
	rosimu_msg.linear_acceleration.y = accY * (8.0 / 65536.0) * 9.81;
	rosimu_msg.linear_acceleration.z = accZ * (8.0 / 65536.0) * 9.81;
	
	imu_pub.publish( &rosimu_msg);
	
	rosimutemp_msg.header.stamp = nh.now();
	rosimutemp_msg.header.frame_id = frameid;
	rosimutemp_msg.temperature = (double)(temp/340)+36.53;
	
	imu_temperature_pub.publish( &rosimutemp_msg);
	
	t.header.frame_id = odom;
	t.child_frame_id = base_link;
	t.transform.translation.x = 1.0;
	t.transform.rotation.x = 0.0;
	t.transform.rotation.y = 0.0;
	t.transform.rotation.z = 0.0;
	t.transform.rotation.w = 1.0;
	t.header.stamp = nh.now();
	broadcaster.sendTransform(t);
	
	
	// Reset the gyro angle to kalman filtered angle when it has drifted beyond center point
	// 	if (gyroXangle < -180 || gyroXangle > 180)
	// 	gyroXangle = kalAngleX;
	// 	if (gyroYangle < -180 || gyroYangle > 180)
	// 	gyroYangle = kalAngleY;
	// 	if (kalAngleX >= -0.05 && kalAngleX <= 0.05) {
	// 		kalAngleX = 0.0;
	// 	}
	// 	if(kalAngleY >= -0.05 && kalAngleY <= 0.05){
	// 		kalAngleY = 0.0;
	// 	}
}
static void MPU_settlewait(double pastKalAngleX, double pastKalAngleY){ //waits until kalAngle is settled
	int settled = 0;
	MPU_update();
	while ((fabs(kalAngleY - pastKalAngleY) > 0.03) || (fabs(kalAngleX - pastKalAngleX) > 0.03))
	{ settled = 1;
		pastKalAngleY = kalAngleY;
		pastKalAngleX = kalAngleX;
		wait(100000);
	MPU_update();       }
}
static void MPU_print(){	//sends roll, and pitch data to mainboard in 1 packet
	Serial.print(F("<X,")); Serial.print(kalAngleX,2);
	Serial.print(F(",")); Serial.print(kalAngleY,2);Serial.print(F(">"));
}
Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
	// Abbreviations for the various angular functions
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);

	Quaternion q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * cp * sr - sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;

	return q;
}
// The setup() function runs once each time the micro-controller starts
void setup()
{
	MPU_init(); //Start MPU
	timer_init(); //Initialize timers
	Serial.begin(115200);	//Start Serial Connection
	pinMode(LED_BUILTIN, OUTPUT);// initialize LED_BUILTIN as an output.
	nh.initNode();
	broadcaster.init(nh);
	
	nh.advertise(imu_pub);
	nh.advertise(imu_temperature_pub);
	
	rosimu_msg.header.frame_id = frameid;
	rosimu_msg.header.stamp = nh.now();
	
	rosimu_msg.orientation.x = q.x;
	rosimu_msg.orientation.y = q.y;
	rosimu_msg.orientation.z = q.z;
	rosimu_msg.orientation.w = q.w;
	
	double orientation_stddev;
	rosimu_msg.orientation_covariance[0] = orientation_stddev;
	rosimu_msg.orientation_covariance[4] = orientation_stddev;
	rosimu_msg.orientation_covariance[8] = orientation_stddev;
	
	double linear_acceleration_stddev;
	rosimu_msg.linear_acceleration_covariance[0] = linear_acceleration_stddev;
	rosimu_msg.linear_acceleration_covariance[4] = linear_acceleration_stddev;
	rosimu_msg.linear_acceleration_covariance[8] = linear_acceleration_stddev;
	
	double angular_velocity_stddev;
	rosimu_msg.angular_velocity_covariance[0] = angular_velocity_stddev;
	rosimu_msg.angular_velocity_covariance[4] = angular_velocity_stddev;
	rosimu_msg.angular_velocity_covariance[8] = angular_velocity_stddev;
	
	rosimutemp_msg.variance = 0;
}

// Add the main program code into the continuous loop() function
void loop()
{
	blinkLED();
	MPU_update(); //only updates when mpucallback is high from timer
	nh.spinOnce();
}
