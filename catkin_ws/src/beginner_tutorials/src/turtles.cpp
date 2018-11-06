
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
float KPx = 1;
float Kp_theta= 3;
using namespace std;
typedef struct
	{
	   
		float x;
		float y;
		float theta;

	}turtles;

 

 turtles turtles1;



	turtlesim::Pose host_mocap, host_mocap2, host_mocap3, host_mocap4;
	void host_pos(const turtlesim::Pose::ConstPtr& msg)
	{

		host_mocap = *msg;

	}
	void host_pos2(const turtlesim::Pose::ConstPtr& msg)
	{

		host_mocap2 = *msg;

	}
	void host_pos3(const turtlesim::Pose::ConstPtr& msg)
	{

		host_mocap3 = *msg;

	}
	void host_pos4(const turtlesim::Pose::ConstPtr& msg)
	{

		host_mocap4 = *msg;

	}  //four turtles

void follow(turtles& turtles1,turtlesim::Pose& host_mocap,geometry_msgs::Twist*t, float dis_x, float dis_y)
{
		float local_x, local_y;

        
		float err_theta;
		float err_long;
		float errx,erry;



		local_x = dis_x; //to middle point
		local_y = dis_y; //1



		err_theta=atan2 ((turtles1.y - host_mocap.y+local_y),(turtles1.x - host_mocap.x+local_x)); //-pi~pi


		if(host_mocap.theta>pi)
		host_mocap.theta = host_mocap.theta - 2*pi;	

		
		
	   	err_theta= err_theta- host_mocap.theta;

		if(err_theta>pi)
		err_theta=err_theta-2*pi;
		else if(err_theta<-pi)
		err_theta=err_theta+2*pi; //turn to -pi~pi*/

		float ux;
	

		errx = (turtles1.x - host_mocap.x + local_x);
		erry = (turtles1.y - host_mocap.y + local_y);

		err_long=sqrt(pow(errx, 2) + pow(erry, 2));

		ux = KPx*err_long;

		t->linear.x=ux;
    	t->angular.z= Kp_theta*err_theta; 

		//ROS_INFO("err_theta1 %f err_theta2 %ferr_theta3 %f err_theta4 %f",err_theta1/pi*180,err_theta2/pi*180,err_theta3/pi*180,err_theta4/pi*180);
		ROS_INFO("turtles1.x %f turtles1.y %f host_mocap.x %f host_mocap.y %f local_x %f local_y %f",turtles1.x,turtles1.y,host_mocap.x,host_mocap.y,local_x,local_y);
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

	ros::init(argc, argv, "turtle");

	ros::NodeHandle n;

	ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, host_pos);
	ros::Subscriber pos_sub2 = n.subscribe<turtlesim::Pose>("turtle2/pose", 10, host_pos2);
	ros::Subscriber pos_sub3 = n.subscribe<turtlesim::Pose>("turtle3/pose", 10, host_pos3);
	ros::Subscriber pos_sub4 = n.subscribe<turtlesim::Pose>("turtle4/pose", 10, host_pos4); //in pos

	ros::Publisher local_vel_pub=n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
	ros::Publisher local_vel_pub2=n.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel",10);
	ros::Publisher local_vel_pub3=n.advertise<geometry_msgs::Twist>("/turtle3/cmd_vel",10);
	ros::Publisher local_vel_pub4=n.advertise<geometry_msgs::Twist>("/turtle4/cmd_vel",10);

	ros::Rate rate(10);




	geometry_msgs::Twist t, t2, t3, t4;
	turtles turtles1;

	turtles1.x=5;
	turtles1.y=5;
	turtles1.theta=0;

	t.linear.x=0;
    t.linear.y=0;
	t.angular.z=0;


	t2.linear.x=0;
    t2.linear.y=0;
	t2.angular.z=0;


	t3.linear.x=0;
   	t3.linear.y=0;
	t3.angular.z=0;


	t4.linear.x=0;
    t4.linear.y=0;
	t4.angular.z=0;


    while (ros::ok() ) {


		
	int c=getch();

	if (c != EOF){
		switch (c){
            case 65:    // key up
                turtles1.y += 1;
                break;
            case 66:    // key down
                turtles1.y += -1;
                break;
            case 67:    // key CW(->)
                turtles1.x += 1;
                break;
			case 68:    // key CCW(<-)
                turtles1.x += -1;
                break;
			case 115:    // key s

				{
				turtles1.x = 1; //return 0
			  	turtles1.y = 1;
                break;
				}

				return 0;
                break;

			}

    	}



		follow(turtles1,host_mocap,&t,0.5,0.5) ;      //be careful of the sign
		follow(turtles1,host_mocap2,&t2,-0.5,-0.5);  //be careful of the sign
		follow(turtles1,host_mocap3,&t3,0.5,-0.5);   //be careful of the sign
		follow(turtles1,host_mocap4,&t4,-0.5,0.5); //be careful of the sign



        local_vel_pub.publish(t);
		local_vel_pub2.publish(t2);
		local_vel_pub3.publish(t3);
		local_vel_pub4.publish(t4);



		ROS_INFO("0 x %f,y %f",turtles1.x,turtles1.y);
		ROS_INFO("1 %f,%f",t.linear.x,t.angular.z);
		ROS_INFO("2 %f,%f",t2.linear.x,t2.angular.z);
		ROS_INFO("3 %f,%f",t3.linear.x,t3.angular.z);
		ROS_INFO("4 %f,%f",t4.linear.x,t4.angular.z);

        ros::spinOnce();
      

   }

	return 0;
}


