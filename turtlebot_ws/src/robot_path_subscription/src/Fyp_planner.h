 /** include the libraries you need in your planner here */
 /** for global path planner interface */
 #include <ros/ros.h>
 #include <costmap_2d/costmap_2d_ros.h>
 #include <costmap_2d/costmap_2d.h>
 #include <nav_core/base_global_planner.h>
 #include <geometry_msgs/PoseStamped.h>
 #include <angles/angles.h>
 #include <base_local_planner/world_model.h>
 #include <base_local_planner/costmap_model.h>
 //#include <mbf_costmap_core/costmap_controller.h>

 using std::string;

 #ifndef FYP_PLANNER_CPP
 #define FYP_PLANNER_CPP

 namespace Fyp_planner {

 class FypPlanner : public nav_core::BaseGlobalPlanner {
 public:

  FypPlanner();
  FypPlanner(std::string name, costmap_2d::Costmap2DROS* costmap_ros);

  /** overridden classes from interface nav_core::BaseGlobalPlanner **/
  void initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros);
  bool makePlan(const geometry_msgs::PoseStamped& start,
                const geometry_msgs::PoseStamped& goal,
                std::vector<geometry_msgs::PoseStamped>& plan
               );
  };
 };
 #endif

 /**
 * @brief Calculates the velocity command based on the current robot pose given by pose. The velocity
 * and message are not set. See the interface in move base flex.
 * @param pose Current robot pose
 * @param velocity
 * @param cmd_vel Output the velocity command
 * @param message
 * @return a status code defined in the move base flex ExePath action.
 */
/*virtual uint32_t computeVelocityCommands(const geometry_msgs::PoseStamped& pose,
                                         const geometry_msgs::TwistStamped& velocity,
                                         geometry_msgs::TwistStamped &cmd_vel,
                                         std::string &message);

/**
 * @brief Returns true, if the goal is reached. Currently does not respect the parameters given.
 * @param dist_tolerance Tolerance in distance to the goal
 * @param angle_tolerance Tolerance in the orientation to the goals orientation
 * @return true, if the goal is reached
 */
/*virtual bool isGoalReached(double dist_tolerance, double angle_tolerance);

/**
 * @brief Canceles the planner.
 * @return True on cancel success.
 */
//virtual bool cancel();

