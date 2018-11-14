#include "ros/ros.h" 
#include <sstream>
#include "std_msgs/String.h"

int main(int argc, char **argv) {
  ros::init(argc, argv, "talker"); 
  ros::NodeHandle n; 
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000); 
  /*
  advertise: tell ROS to publish
  std_msgs::String:type
  publish topic name: chatter.
    max message:1000比 
  */
  ros::Rate loop_rate(10);     //10Hz
  int count = 0; 

  while (ros::ok())
  {
    std_msgs::String msg;
    std::stringstream ss; 
    ss << "I love NCRL " << count; 
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str()); 

    chatter_pub.publish(msg); 
    ros::spinOnce();      //ROS return messsage 關掉 Ctrl+C 
    loop_rate.sleep();
    ++count; 
  }
  return 0;
}

