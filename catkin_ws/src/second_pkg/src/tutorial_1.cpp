/*
  Author : Vivan
  Target : Let turtlesim go straight forward with speed 1 m/s
  Annotator : Chad Lin
  Date : 11/07 2018
*/

// include ros library
#include "ros/ros.h"

// include msg library
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>

turtlesim::Pose pose;
geometry_msgs::Twist vel_msg;
// declare call back function
void pos_cb(const turtlesim::Pose::ConstPtr& msg)
{
  pose = *msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tutorial_1");
  ros::NodeHandle n;

  // declare publisher & subscriber
  ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, pos_cb);
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);

  // setting frequency as 10 Hz
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()){
    printf("pos x : %f\t y: %f\n",pose.x,pose.y);

    vel_msg.linear.x= 1.0;
    vel_msg.linear.y= 0;
    vel_msg.linear.z= 0;

    vel_msg.angular.x= 0;
    vel_msg.angular.y= 0;
    vel_msg.angular.z= 0.5;

    turtlesim_pub.publish(vel_msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}



