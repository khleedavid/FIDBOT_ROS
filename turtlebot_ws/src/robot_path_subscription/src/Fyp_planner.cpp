#include <pluginlib/class_list_macros.h>
#include "Fyp_planner.h"
#include <tf/transform_broadcaster.h>
#include <ros/ros.h>
#include <nav_msgs/Path.h>


 //register this planner as a BaseGlobalPlanner plugin
 PLUGINLIB_EXPORT_CLASS(Fyp_planner::FypPlanner, nav_core::BaseGlobalPlanner)

 using namespace std;

 nav_msgs::Path gPlan;

 //Default Constructor
 namespace Fyp_planner {

 FypPlanner::FypPlanner (){

 }

 FypPlanner::FypPlanner(std::string name, costmap_2d::Costmap2DROS* costmap_ros){
   initialize(name, costmap_ros);
 }


 void FypPlanner::initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros){

 }

 void mapCallback(const nav_msgs::Path::ConstPtr& msg)
    {
     
      int i=0;
      for(std::vector<geometry_msgs::PoseStamped>::const_iterator it= msg->poses.begin(); it!= msg->poses.end(); ++it)
      {
        

        gPlan.poses[i] = *it;
        i++;
       
      }
    }

int main(int argc, char **argv)
    {
      ros::init(argc, argv, "listener");
      ros::NodeHandle n;
      ros::Subscriber sub = n.subscribe("room_exploration/room_exploration_server/coverage_path", 100, mapCallback);
      ros::spin();

      return 0;
    }

 bool FypPlanner::makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,  std::vector<geometry_msgs::PoseStamped>& plan ){
      
         plan.push_back(start);
        for(int i=0; i<=gPlan.poses.size(); i++)  {
          geometry_msgs::PoseStamped new_goal = goal;
          new_goal.pose.position = gPlan.poses[i].pose.position;
          new_goal.pose.orientation = gPlan.poses[i].pose.orientation;
         
      plan.push_back(new_goal); 
      }
      plan.push_back(goal);
      return true;
    
    
    


    /*double radius = 4; //radius of circle
    double resolution = 0.5; //resolution of circle in radians
    double index = resolution; //how much to increment at each point on the circle
    int full_circle = 6/resolution; //how many increments to finish the circle eg. 6 radians is a full circle


    int step = 5; //resolution of the straight part from center to point on circle
    int for_loop=0; //array location counter
    double increment = radius/step; 
    int size = step + full_circle;      


    double x_[size]={0};
    double y_[size]={0};
    double angle;*/

   /*//coordinates from center to a point on the circle
   for( int w = 0; w < step; w++){ 

    x_[w]=increment;
    y_[w]=0;
    increment = increment + radius/step;
    for_loop++;

   }*/

   /*//coordinates from the point on the circle to the full circle
   for( int s = for_loop; s < size + 1; s++){

    angle = angle + index;
    x_[s] = radius*cos(angle);
    y_[s] = radius*sin(angle);       

   }*/



  /*  plan.push_back(start);

   for(int i = 0; i < size + 1; i++){
    geometry_msgs::PoseStamped new_goal = goal;
    tf::Quaternion goal_quat = tf::createQuaternionFromYaw(3.14);

    new_goal.pose.position.x = poseStamped[i].pose.position.x;
    new_goal.pose.position.y = poseStamped[i].pose.position.y;

    new_goal.pose.orientation.x = goal_quat.x();
    new_goal.pose.orientation.y = goal_quat.y();
    new_goal.pose.orientation.z = goal_quat.z();
    new_goal.pose.orientation.w = goal_quat.w();

    plan.push_back(new_goal);

   } 

  return true;*/
 }
 };