#include "ros.h"

#include "geometry_msgs/Twist.h"

#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
float x;
long i;
long step_count;
long delayrotate;
float rotate;
int level;
ros::NodeHandle nh;
int dir;//0: forward,-1:left, 1 right
void velCallback(  const geometry_msgs::Twist& vel)
{
  x = vel.linear.x; // I CAN USE VEL AS I WANT
 
  rotate = vel.angular.z;
  if (rotate < 0) {
    dir = -1;
  }
  if (rotate > 0) {
    dir = 1;
  }
  if (rotate == 0)
    dir = 0;
  float absHR = rotate < 0 ? -rotate : rotate;
  if (0 < absHR && absHR <= PI / 6)
    level = 2;
  else if (PI / 6 < absHR && absHR <= PI / 3)
    level = 1;
  else if (absHR == 0) level = 3;
  else level = 0;
  if(x==0&&dir!=0)
    x=0.2;
     step_count = x / 0.000471;
  delayrotate = 1000000 / step_count;
  
  
  //in log
  char log_msg[50];
  char str_temp[6];
  dtostrf(x, 4, 2, str_temp);
  sprintf(log_msg,"x=%s,level=%d,dir=%d",str_temp,level,dir);
  nh.loginfo(log_msg);

}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);

void setup() {
  Serial.begin(9600);
  nh.initNode();
  nh.subscribe(sub);
  x = 0;
  rotate = 0;
  step_count = 0;

  pinMode(STEP1, OUTPUT); // Step
  pinMode(DIR1, OUTPUT); // Dir - chiều quay
  pinMode(STEP2, OUTPUT); // Step
  pinMode(DIR2, OUTPUT); // Dir - chiều quay
}

void loop() {
  nh.spinOnce();

    
  digitalWrite(DIR1, LOW); // Set Dir high  - Set chiều quay mức cao
  digitalWrite(DIR2, HIGH); // Dir - chiều quay
  if (dir == 0) {
    for (i = 0; i < step_count; i++) 
    {
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(delayrotate);         // Trễ 1ms
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(delayrotate);         // Trễ 3ms
    }
  }
  else if (dir > 0) {
    for (i = 0; i < step_count; i++) 
    {
      if (i % 3 <= level)
        digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(delayrotate);
      // Trễ 1ms
      if (i % 3 <= level)
        digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(delayrotate);         // Trễ 3ms
    }
  }
  else {
    for (i = 0; i < step_count; i++) 
    {

      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      if (i % 3 <= level)
        digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(delayrotate);
      // Trễ 1ms

      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      if (i % 3 <= level)
        digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(delayrotate);         // Trễ 3ms
    }
  }
  x=0;
  dir=0;
  step_count=0;

}
