/*
  Author : Chad Lin
  Target : Let turlesim go to goal point
  Date : 11/07 2018
*/

// include ros library
#include "ros/ros.h"

// include msg library
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>

// include math 
#include <math.h>

using namespace std;

turtlesim::Pose pose;
geometry_msgs::Twist vel_msg;
geometry_msgs::Point goal_point;

struct XYZ{
  float x;
  float y;
  float z;
};

struct XYZ pos_err_I;

// declare call back function
void pos_cb(const turtlesim::Pose::ConstPtr& msg)
{
  pose = *msg;
}

void rotate2D(float &x, float &y, float theta)
{
  float x1 = x;
  float y1 = y;
  x = cos(theta) * x1 - sin(theta) * y1;
  y = sin(theta) * x1 + cos(theta) * y1;
} 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tutorial_1");
  ros::NodeHandle n;

  // declare publisher & subscriber
  ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, pos_cb);
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);

  goal_point.x = 3;
  goal_point.y = 7;
  // setting frequency as 10 Hz
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()){

    printf("\ncount : %d\n",count);
    printf("goal x : %f \t y : %f\n",goal_point.x,goal_point.y);
    printf("pose x : %f \t y : %f\n",pose.x,pose.y);

    // err in inertia frame
    pos_err_I.x = goal_point.x - pose.x;
    pos_err_I.y = goal_point.y - pose.y;
    
    // rotate to body frame
    rotate2D(pos_err_I.x, pos_err_I.y, -pose.theta);

    vel_msg.linear.x = pos_err_I.x;
    vel_msg.angular.z = 0.5 * pos_err_I.y;
    turtlesim_pub.publish(vel_msg);

    count ++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}



