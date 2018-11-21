#include <ros/ros.h>			//include ros library
int main(int argc, char **argv)
{
ros::init(argc, argv, "test");     	//NODE name
ros::NodeHandle n;	  			 //NodeHandle n: start node 
ROS_INFO("Hello world!");		//print
}

