
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
float Kp_theta= 2;
using namespace std;
typedef struct
	{
	   
		float x;
		float y;
		float theta;

	}turtles;

 

 turtles turtles1;
typedef struct 
	{
		float x;
		float y;

	}displacement;


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

	void follow(turtles& turtles1,turtlesim::Pose& host_mocap,geometry_msgs::Twist*t,displacement &dis_host,turtlesim::Pose& nbr_mocap,displacement &dis_nbr,turtlesim::Pose&nbr_mocap2,displacement&dis_nbr2,turtlesim::Pose& nbr_mocap3,displacement &dis_nbr3)
{
		float local_x, local_y;
		float local_x1, local_y1;
		float local_x2, local_y2;
		float local_x3, local_y3;


		float dis_x1, dis_y1;
		float dis_x2, dis_y2;
		float dis_x3, dis_y3;


        
		float err_theta;
		//float err_theta1,err_theta2,err_theta3,err_theta4;
		float err_long;
		float errx, erry;
		float errx1, errx2, errx3, errx4;
		float erry1, erry2, erry3, erry4;



		local_x = dis_host.x; //to middle point
		local_y = dis_host.y; //1

		dis_x1=dis_host.x-dis_nbr.x; //dis.nbr :another neibor to middle point
		dis_y1=dis_host.y-dis_nbr.y;

		local_x1= dis_x1; //host-nbr1
		local_y1= dis_y1;//2

		dis_x2=dis_host.x-dis_nbr2.x; 
		dis_y2=dis_host.y-dis_nbr2.y;

		local_x2= dis_x2;//host-nbr2
		local_y2= dis_y2;//3

		dis_x3=dis_host.x-dis_nbr3.x;
		dis_y3=dis_host.y-dis_nbr3.y;

		local_x3= dis_x3;//host-nbr3
		local_y3= dis_y3;//4

		float az;

		err_theta=atan2 (erry1,errx1); //-pi~pi
		

		/*err_theta1=atan2 ((turtles1.y - host_mocap.y+local_y),(turtles1.x - host_mocap.x+local_x)); //-pi~pi
		err_theta2=atan2((nbr_mocap.y-host_mocap.y+local_y1),(nbr_mocap.x-host_mocap.x+local_x1));
		err_theta3=atan2((nbr_mocap2.y-host_mocap.y+local_y2),(nbr_mocap2.x-host_mocap.x+local_x2));
		err_theta4=atan2((nbr_mocap3.y-host_mocap.y+local_y3),(nbr_mocap3.x-host_mocap.x+local_x3));*/


		if(host_mocap.theta>pi)
		host_mocap.theta -= 2*pi;	

		err_theta-=host_mocap.theta; //no interaction
		

		if(err_theta>pi)
		err_theta-=2*pi;
		else if(err_theta<-pi)
		err_theta+=2*pi; //turn to -pi~pi*/
		
		az=Kp_theta*err_theta;


		float ux;
	

		errx1 = turtles1.x - host_mocap.x + local_x;
		errx2 =nbr_mocap.x-host_mocap.x+local_x1;
		errx3 =nbr_mocap2.x-host_mocap.x+local_x2;
		errx4 =nbr_mocap3.x-host_mocap.x+local_x3;

		//errx = errx1;  
		errx=0.25*errx1+0.25*errx2+0.25*errx3+0.25*errx4;

		erry1 = turtles1.y - host_mocap.y + local_y;
		erry2 = nbr_mocap.y-host_mocap.y+local_y1;
		erry3 = nbr_mocap2.y-host_mocap.y+local_y2;
		erry4 = nbr_mocap3.y-host_mocap.y+local_y3;
		//erry=erry1;  
		erry=0.25*erry1+0.25*erry2+0.25*erry3+0.25*erry4;

		err_long=sqrt(pow(errx, 2) + pow(erry, 2));

	
		//ROS_INFO("errx %f erry %f",errx,erry);

		ux = KPx*err_long;


		t->linear.x=ux;
    	t->angular.z= az; 

		//ROS_INFO("err_theta1 %f err_theta2 %ferr_theta3 %f err_theta4 %f",err_theta1/pi*180,err_theta2/pi*180,err_theta3/pi*180,err_theta4/pi*180);
		ROS_INFO("turtles1 %f,%f host_mocap %f,%f",turtles1.x,turtles1.y,host_mocap.x,host_mocap.y);
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

	ros::init(argc, argv, "turtles_interact_changevir");

	ros::NodeHandle n;

	ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, host_pos);
	ros::Subscriber pos_sub2 = n.subscribe<turtlesim::Pose>("turtle2/pose", 10, host_pos2);
	ros::Subscriber pos_sub3 = n.subscribe<turtlesim::Pose>("turtle3/pose", 10, host_pos3);
	ros::Subscriber pos_sub4 = n.subscribe<turtlesim::Pose>("turtle4/pose", 10, host_pos4); //in pos

	ros::Publisher local_vel_pub=n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
	ros::Publisher local_vel_pub2=n.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel",10);
	ros::Publisher local_vel_pub3=n.advertise<geometry_msgs::Twist>("/turtle3/cmd_vel",10);
	ros::Publisher local_vel_pub4=n.advertise<geometry_msgs::Twist>("/turtle4/cmd_vel",10);

	ros::Rate loop_rate(10);



	geometry_msgs::Twist t, t2, t3, t4;
	turtles turtles1;
	displacement dis1, dis2, dis3, dis4;

	dis1.x=0.5;
	dis1.y=0.5;

	dis2.x=-0.5;
	dis2.y=-0.5;

	dis3.x=0.5;
	dis3.y=-0.5;

	dis4.x=-0.5;
	dis4.y=0.5;



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

	
//void follow(turtles& turtles1, turtlesim::Pose& host_mocap, geometry_msgs::Twist* t, turtlesim::Pose& nbr_mocap, displacement &dis_nbr, turtlesim::Pose& nbr_mocap2, displacement &dis_nbr2, turtlesim::Pose& nbr_mocap3, displacement &dis_nbr3)
        follow(turtles1,host_mocap,&t,dis1,host_mocap2,dis2,host_mocap3,dis3,host_mocap4,dis4);
       
        follow(turtles1,host_mocap2,&t2,dis2,host_mocap,dis1,host_mocap3,dis3,host_mocap4,dis4); 
   
        follow(turtles1,host_mocap3,&t3,dis3,host_mocap2,dis2,host_mocap,dis1,host_mocap4,dis4); 

        follow(turtles1,host_mocap4,&t4,dis4,host_mocap2,dis2,host_mocap3,dis3,host_mocap,dis1); 


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
       loop_rate.sleep();

	   }
  

	return 0;
}


