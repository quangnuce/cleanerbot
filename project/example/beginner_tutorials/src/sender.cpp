#include <stdio.h>
#include <sstream>
#include "ros/ros.h"
#include "beginner_tutorials/IntsAndText.h"

// Main loop
int main(int argc, char **argv)
{
  // Initialize the node
  ros::init(argc, argv, "adder");
  ros::NodeHandle n;
  
  // Create a publisher
  ros:: Publisher pub = n.advertise<beginner_tutorials::IntsAndText>("addition", 100);
  
  // Create a rate
  ros::Rate rate(1); // 1 message per second

  // Declare variables
  int a, b, sum;

  // Load in values
  a = 1;
  b = 2;

  // Run our code in a loop until node is shutdown
  while (ros::ok())
  {
    // Compute sum
    sum = a + b;

    // Create string stream explaining result
    std::stringstream text;
    text << a << " + " << b << " = " << sum;
    
    // Create message
    beginner_tutorials::IntsAndText message;
    
    // Load in message components
    message.first = a;
    message.second = b;
    message.third = sum;
    message.text = text.str();
    
    // Publish the message
    pub.publish(message);
    
    // Sleep to fix loop rate
    rate.sleep();
  }
}
