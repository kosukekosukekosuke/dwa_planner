#ifndef __DWA_PLANNER_H
#define __DWA_PLANNER_H

#include <ros/ros.h>
#include <tf/tf.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <Eigen/Dense>

class DWAPlanner
{
public:
    DWAPlanner(void);

    class State
    {
    public:
        State(double, double, double, double, double);

        double x;// robot position x
        double y;// robot posiiton y
        double yaw;// robot orientation yaw
        double velocity;// robot linear velocity
        double yawrate;// robot angular velocity
    private:
    };

    class Window
    {
    public:
        Window(void);
        Window(const double, const double, const double, const double);
        double min_velocity;
        double max_velocity;
        double min_yawrate;
        double max_yawrate;
    private:
    };
    void process(void);
    void local_goal_callback(const geometry_msgs::PoseStampedConstPtr&);
    void local_map_callback(const nav_msgs::OccupancyGridConstPtr&);
    void odom_callback(const nav_msgs::OdometryConstPtr&);
    void target_velocity_callback(const geometry_msgs::TwistConstPtr&);
    void calc_dynamic_window(Window&, const geometry_msgs::Twist&);
    float calc_to_goal_cost(const std::vector<State>& traj, const Eigen::Vector3d& goal);
    float calc_speed_cost(const std::vector<State>& traj, const float target_velocity);
    float calc_obstacle_cost(const std::vector<State>& traj, const nav_msgs::OccupancyGrid& map);
    void motion(State& state, const double velocity, const double yawrate);
    void visualize_trajectories(const std::vector<std::vector<State>>&, const double, const double, const double, const int, const ros::Publisher&);
    void visualize_trajectory(const std::vector<State>&, const double, const double, const double, const ros::Publisher&);

protected:
    double HZ;
    std::string ROBOT_FRAME;
    double TARGET_VELOCITY;
    double MAX_VELOCITY;
    double MIN_VELOCITY;
    double MAX_YAWRATE;
    double MAX_ACCELERATION;
    double MAX_D_YAWRATE;
    double MAX_DIST;
    double VELOCITY_RESOLUTION;
    double YAWRATE_RESOLUTION;
    double ANGLE_RESOLUTION;
    double PREDICT_TIME;
    double TO_GOAL_COST_GAIN;
    double SPEED_COST_GAIN;
    double OBSTACLE_COST_GAIN;
    double DT;

    ros::NodeHandle nh;
    ros::NodeHandle local_nh;

    ros::Publisher velocity_pub;
    ros::Publisher candidate_trajectories_pub;
    ros::Publisher selected_trajectory_pub;
    ros::Subscriber local_map_sub;
    ros::Subscriber local_goal_sub;
    ros::Subscriber odom_sub;
    ros::Subscriber target_velocity_sub;
    tf::TransformListener listener;

    Window dynamic_window;
    geometry_msgs::PoseStamped local_goal;
    nav_msgs::OccupancyGrid local_map;
    geometry_msgs::Twist current_velocity;
    bool local_goal_subscribed;
    bool local_map_updated;
    bool odom_updated;
};

#endif //__DWA_PLANNER_H
