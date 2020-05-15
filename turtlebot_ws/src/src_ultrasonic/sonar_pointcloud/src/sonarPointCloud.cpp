#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/String.h"

#define MAX_POINTS 20000

typedef pcl::PointCloud<pcl::PointXYZI> PointCloud;

float Arr[541];

void arrayCallback(const std_msgs::Float32MultiArray::ConstPtr& array);


int main( int argc, char** argv )
{
	ros::init(argc, argv, "pointCloud");
	ros::NodeHandle n;

	//Publish
	//ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
	ros::Publisher pc_pub = n.advertise<PointCloud> ("sonarPC", 100);	

	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatterPC", 1000);
	char hello[13] = "hello world!";

	//Subscribe
	ros::Subscriber pc_sub = n.subscribe("sonarPCarray", 100, arrayCallback);

	//Set loop rate (times per second)
	ros::Rate loop_rate(30);

	PointCloud::Ptr msg (new PointCloud);
	msg->header.frame_id = "base_sonar";
	msg->height = 1;
	msg->width = MAX_POINTS;
	msg->points.resize (msg->width * msg->height);


	int counter = 0;

	while (ros::ok())
	{
		for(int i = 0; i < 90; i ++)
		{
				//Ignore its own reflections, also threshold.
				if( (i>=10) && (Arr[3+(i*4)] >= 90.0) )
				{
					msg->points[counter].x = Arr[0+(i*4)];	//X point
					msg->points[counter].y = Arr[2+(i*4)];	//Y point (unused)
					msg->points[counter].z = Arr[1+(i*4)];  //Z point

					msg->points[counter].intensity = Arr[3+(i*4)];
					//msg->points[counter].g = int(Arr[4+(i*6)]);
					//msg->points[counter].b = int(Arr[5+(i*6)]);

					msg->header.stamp = pcl_conversions::toPCL(ros::Time::now());
				
					pc_pub.publish(msg);
							
					counter ++;
					//i = 90;
				}
		}
		std_msgs::String str_msg;

      	str_msg.data = hello;
      	chatter_pub.publish(str_msg);
		ROS_INFO("PointCloud ACTIVE");		
		ros::spinOnce();
		loop_rate.sleep();

		if(counter >= MAX_POINTS)
		{
			counter = 0;
		}


	}
}

/*************************************************
** Returns the X Pose				**
*************************************************/

void arrayCallback(const std_msgs::Float32MultiArray::ConstPtr& array)
{

	 int i = 0;		
	for(std::vector<float>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		Arr[i] = *it;
		i++;
	}
	return;
}
