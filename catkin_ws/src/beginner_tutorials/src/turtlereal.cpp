

#include "ros/ros.h"
#include <math.h>
#include <geometry_msgs/Pose.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>



char getch()
{
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    if (read(0, &buf, 1) < 0) {
        //perror ("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror ("tcsetattr ~ICANON");
    }
    return (buf);
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "turtlereal");

    ros::NodeHandle n;

    ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);

    ros::Rate loop_rate(100);

    geometry_msgs::Twist vel_msg;


    vel_msg.linear.x= 0;
    vel_msg.linear.y= 0;
    vel_msg.linear.z= 0;

    vel_msg.angular.x= 0;
    vel_msg.angular.y= 0;
    vel_msg.angular.z= 0;


    while (ros::ok())
    {

        int c = getch();

        if (c != EOF) {
            switch (c) {
            case 65:    // key up
                vel_msg.linear.x+= 0.01;
                break;
            case 66:    // key down
                vel_msg.linear.x += -0.01;
                break;
            case 67:    // key CW(->)
                vel_msg.angular.z += 0.01;
                break;
            case 68:    // key CCW(<-)
                vel_msg.angular.z += -0.01;
                break;
                
            case 115:
                vel_msg.linear.x = 0;
				vel_msg.angular.z=0; //key s
                break;

                return 0;
                break;
            }

        }
        ROS_INFO("%f,%f",vel_msg.linear.x,vel_msg.angular.z);
        //ROS_INFO("%f,%f",pos.x,pos.y);
        //ROS_INFO("%f,%f,%f",err_theta/pi*180,err_theta1/pi*180,host_mocap.theta/pi*180);

        turtlesim_pub.publish(vel_msg);

        ros::spinOnce();
        loop_rate.sleep();

    }




    return 0;
}

