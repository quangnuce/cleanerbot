/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2009, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Eitan Marder-Eppstein
*********************************************************************/
#include <ros/ros.h>
#include "sensor_msgs/Range.h"
#include <sensor_msgs/LaserScan.h>
unsigned int num_readings = 150;
  double laser_frequency = 1000;
  double ranges[150];
  double intensities[150];
  int overround=0;
 ros::Publisher scan_pub;
ros::Subscriber sub;
void rangeCallback(const sensor_msgs::Range::ConstPtr& range_data)
{
  //range = range_data->range;
int angle=(int)(range_data->field_of_view)-15;
	ranges[(int)(range_data->field_of_view)-15]=range_data->range;
  ROS_INFO("I heard: [%f],[%f]", range_data->range,range_data->field_of_view);
	
	if(angle==0){
		ROS_INFO("broadcast /scan");
		for(unsigned int i = 0; i < num_readings; ++i){
    			//  ranges[i] = count;
      			intensities[i] =0;
    		}
    		ros::Time scan_time = ros::Time::now();

    		//populate the LaserScan message
    		sensor_msgs::LaserScan scan;
    		scan.header.stamp = scan_time;
    		scan.header.frame_id = "laser_frame";
    		scan.angle_min = -1.3089;
    		scan.angle_max = 1.3089;
    		scan.angle_increment = 2.617 / num_readings;
    		scan.time_increment = 50;
    		scan.range_min = 0.2;
    		scan.range_max = 4.0;

    		scan.ranges.resize(num_readings);
    		scan.intensities.resize(num_readings);
    		for(unsigned int i = 0; i < num_readings; ++i){
      			scan.ranges[i] = ranges[i];
      			scan.intensities[i] = intensities[i];
    		}

    		scan_pub.publish(scan);	
	}


}

int main(int argc, char** argv){
   ros::init(argc, argv, "laser_scan_publisher");
ros::NodeHandle n;
  scan_pub= n.advertise<sensor_msgs::LaserScan>("scan", 1000);
 sub= n.subscribe("range_data", 50, rangeCallback);

  int count = 0;
 ros::Rate loop_rate(1000);
  ros::spin();
 
}

