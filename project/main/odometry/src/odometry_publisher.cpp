#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
geometry_msgs::Pose pose;
geometry_msgs::Pose prepose;
ros::Time current_time, last_time;
 double vx = 0.0;
  double vy = 0.0;
  double vth = 0.0;

void p_sub (const geometry_msgs::PoseStamped::ConstPtr& p_msg) {
	prepose=pose;
       pose=p_msg->pose;
	last_time = current_time;
	current_time = ros::Time::now();
 	double dt = (current_time - last_time).toSec();
	vx=(pose.position.x-prepose.position.x)/dt;
	vy=(pose.position.y-prepose.position.y)/dt;
	tf::Quaternion q(prepose.orientation.x, prepose.orientation.y, prepose.orientation.z, prepose.orientation.w);
	tf::Matrix3x3 m(q);
	double roll, pitch, oyaw,yaw;
	m.getRPY(roll, pitch, oyaw);
	tf::Quaternion q2(pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w);
	tf::Matrix3x3 m2(q2);	
	m2.getRPY(roll, pitch, yaw);
        vth=yaw-oyaw;

}
int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;
  ros::Subscriber cmd_vel=n.subscribe("slam_out_pose" , 10 ,p_sub);


 
  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1.0);
  while(n.ok()){

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();


    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = pose.orientation;

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = pose.position.x;
    odom_trans.transform.translation.y = pose.position.y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose=pose;
	
    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}

