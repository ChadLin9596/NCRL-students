

#include "ros/ros.h"
#include <math.h>
#include <geometry_msgs/Pose.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>

#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define pi 3.1415926
float Kp_theta=100;
float Kpx=1;

	typedef struct
	{
	   
		float x;
		float y;
		float z;
	}turtlewant;

	turtlesim::Pose host_mocap;
	void host_pos(const turtlesim::Pose::ConstPtr& msg) 
	{

		host_mocap = *msg;

	}
	
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

	  ros::init(argc, argv, "turtlesim_cmd");

	  ros::NodeHandle n;

          ros::Subscriber state_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, host_pos);
          ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

  
	  	ros::Rate loop_rate(100);
		turtlewant pos;

		pos.x=5;
		pos.y=5;//while 外

	  while (ros::ok())	 
	{
	
        float err_theta;
		float err_theta1;
		float err_long;
		


		geometry_msgs::Twist vel_msg;
		geometry_msgs::Pose pos_msg;
		

		err_theta1=atan2 ((pos.y - host_mocap.y),(pos.x - host_mocap.x)); //-pi~pi

		if(host_mocap.theta>pi)
		host_mocap.theta = host_mocap.theta - 2*pi;	//turn to 0~2*pi
	    err_theta=err_theta1-host_mocap.theta;

		if(err_theta>pi)
		err_theta=err_theta-2*pi;
		if(err_theta<-pi)
		err_theta=err_theta+2*pi;
			
		
	
   		 	vel_msg.angular.z= Kp_theta*err_theta;
		   	vel_msg.linear.x= Kpx*err_long ;
		




		err_long=sqrt(pow((pos.x - host_mocap.x), 2) + pow((pos.y - host_mocap.y), 2)); //>0

		//vel_msg.linear.x= Kpx*err_long;


        int c = getch();
		//ROS_INFO("C: %d",c);
        if (c != EOF) {
            switch (c) {
            case 65:    // key up
				pos.y += 1;
                break;
            case 66:    // key down

				pos.y -= 1;
                break;
            case 67:    // key CW(->)
                pos.x += 1;
                break;
			case 68:    // key CCW(<-)
                pos.x += -1;
                break;
			case 115:    // key s

				{
				pos.x = 0; //return 0
			  	pos.y = 0;
                break;
				}
			

            
			}
    	}

 
	//ROS_INFO("%f,%f",pos.x,pos.y); 
	//ROS_INFO("%f,%f,%f",err_theta/pi*180,err_theta1/pi*180,host_mocap.theta/pi*180); 

    turtlesim_pub.publish(vel_msg);


    ros::spinOnce();
    //loop_rate.sleep();

  }

  return 0;
}

