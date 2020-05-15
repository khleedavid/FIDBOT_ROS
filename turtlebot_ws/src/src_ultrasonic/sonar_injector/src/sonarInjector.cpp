#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32MultiArray.h"

#define WIDTH	180
#define HEIGHT 	180
#define DEPTH	255

#define SONARRANGE 75.0
#define SONARBINS 90

#define PI 3.14159265

float genRandFloat( float a, float b );
void bearingCB(const std_msgs::Float32::ConstPtr& sonarBearing);
void binsArrCB(const std_msgs::Float32MultiArray::ConstPtr& sonarBinsArr);
void pixelPlace( float theta, float distance, unsigned int depth );

float imgx, imgy, imgz;
float bearing, oldbearing;
int binsArr[90];
float sonarDist = 0;

int main(int argc, char **argv)
{
    
	//init this
	ros::init(argc, argv, "injectPoints");

	//Set up node handle
	ros::NodeHandle n;

	//Publish
	ros::Publisher pub = n.advertise<std_msgs::Float32MultiArray>("sonarPCarray", 100);

	ros::Subscriber sub1 = n.subscribe("sonarBearing", 100, bearingCB);
	ros::Subscriber sub2 = n.subscribe("sonarBinsArr", 100, binsArrCB);

	//set up variables to be published
	std_msgs::Float32MultiArray array;

	ros::Rate r(30);
	
	sonarDist = SONARRANGE / SONARBINS;

	while (ros::ok())
	{

		//Clear array
		array.data.clear();
		for(int i = 0; i < 90; i ++)
		{
			pixelPlace(bearing, float(i), binsArr[i]);

			

			array.data.push_back(imgx);	//x
			array.data.push_back(imgy);	//y
			array.data.push_back(0.0);	//z

			array.data.push_back(imgz);	//i
			//array.data.push_back(0.0);	//g
			//array.data.push_back(0.0);	//b

			//Publish array
			pub.publish(array);
	
			//printf("%f, %f, %f - %f %f %f\n", array.data[0], array.data[1],array.data[2],array.data[3],array.data[4],array.data[5]);
		}
		
		//Let the world know
		//printf("%f, %f, %f - %f %f %f\n", array.data[0], array.data[1],array.data[2],array.data[3],array.data[4],array.data[5]);
		//ROS_INFO("I published something!");
		//Do this.
		ros::spinOnce();
		//Added a delay so not to spam
		r.sleep();

	}

	sleep(1);	

}

float genRandFloat( float a, float b )
{
    return ( (b-a)*( (float)rand() / RAND_MAX ) )+a;
}

/*************************************************
** Returns the sonar bearing **
*************************************************/

void bearingCB(const std_msgs::Float32::ConstPtr& sonarBearing){
bearing = sonarBearing->data;
return;
}

/*************************************************
** Returns the sonar bearing **
*************************************************/

void binsArrCB(const std_msgs::Float32MultiArray::ConstPtr& sonarBinsArr)
{

int i = 0;
  // print all the remaining numbers
  for(std::vector<float>::const_iterator it = sonarBinsArr->data.begin(); it != sonarBinsArr->data.end(); ++it)
  {
    binsArr[i] = *it;
    i++;
  }

return;
}

/****************************************************************
 * Do trig to get the xy coordinates of the thing
 * add on half width and half height so it's centred and some things need
 * flipping to make them right 
 * *********************************************************/
void pixelPlace( float theta, float distance, unsigned int depth )
{

	distance = sonarDist * distance;  //get the actual distance
	float x, y;
	theta /= 17.7777778;
	if( theta > 0 && theta < 90 )
	{

		theta = 90.0 - theta;
		//printf("< 90\n");
		x = (float)distance * cos( theta *PI/180);
		y = (float)distance * sin( theta *PI/180);
		//printf("%f = %f -- %f\n",theta, x, y);
		imgx = x;// + ( (float) WIDTH / 2.0) ;
		imgy = (y * -1);//+ ( (float) HEIGHT / 2.0);

	}
	else if( theta > 91 && theta < 180 )
	{
		//printf("> 90 && < 180\n");
		y = (float)distance * cos(theta *PI/180);
		x = (float)distance * sin(theta *PI/180);
		//printf("%f = %f -- %f\n",theta, x, y);
		imgx = x;// + (WIDTH / 2);
		imgy = (y * -1);// + (HEIGHT / 2);		

	}
	else if( theta > 181 && theta < 270 )
	{

		theta = 90.0 - theta;
		//printf("> 181 && < 270\n");
		x = (float)distance * cos(theta *PI/180);
		y = (float)distance * sin(theta *PI/180);		
		//printf("%f = %f -- %f\n",theta, x, y);
		imgx = x;// + ((float)WIDTH / 2);
		imgy = (y * -1);// + ((float)HEIGHT / 2);

	}
	else
	{
		//printf("> 271 && < 360\n");
		y = (float)distance * cos(theta *PI/180);
		x = (float)distance * sin(theta *PI/180);		
		//printf("%f = %f -- %f\n",theta, x, y);
		imgx = x;// + ((float)WIDTH / 2);
		imgy = (y * -1);// + ((float)HEIGHT / 2);
	}

	//imageArray[y][x] = 1;
	//if(depth > THRESHOLD)
	//	imageArray[imgx][imgy] = 255;
	//else
	//	imageArray[imgx][imgy] = 0;

	imgz = float(depth);
	//imageArray[imgx][imgy] = depth;	
}
