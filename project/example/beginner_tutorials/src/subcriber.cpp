#include <stdio.h>
#include <sstream>
#include "ros/ros.h"
#include "beginner_tutorials/IntsAndText.h"
#include "std_msgs/Int32.h"

/**
 * Callback function which is called everytime a message is published
 * on the "split_me" topic. That message is passed as the argument "msg"
 */
void split_callback(const beginner_tutorials::IntsAndText::ConstPtr& msg)
{
  // Create divider to split the integer
  //int splitter = 2;
  
  // Split the integer stored in the message by dereferencing the data attribute
  //int part_1 = msg->data / splitter;
 // int part_2 = msg->data - part_1;
  
  // Print out the reuslt using ROS_INFO so that the print also gets logged
  ROS_INFO("%d becomes %d and %d", msg->third, msg->first, msg->second);
}

// Main loop
int main(int argc, char **argv)
{
  // Initialize the node
  ros::init(argc, argv, "subadder");
  ros::NodeHandle n;
  


  
  // Create a subscriber
  ros::Subscriber sub = n.subscribe("addition", 100, split_callback);
  
  ros::spin();
}
