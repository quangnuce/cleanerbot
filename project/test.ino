#include "ros.h"
#include "geometry_msgs/Twist.h"
#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
float x,z;
long step1,kc;
ros::NodeHandle nh;
void velCallback(  const geometry_msgs::Twist& vel_cmd)
{
     x = vel_cmd.linear.x; 
     z = vel_cmd.angular.z;
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);
void setup()

{
 nh.initNode();
 nh.subscribe(sub);
pinMode(STEP1,OUTPUT); // Step
pinMode(DIR1,OUTPUT); // Dir - chiều quay
pinMode(STEP2,OUTPUT); // Step
pinMode(DIR2,OUTPUT); // Dir - chiều quay

}
void tinh (float x)
  {
     step1=x/0.00047124;
     kc= 1000/step1; 
  }
void loop()
{
nh.spinOnce();
tinh (x);
Serial.println(step1);
Serial.print(kc);
digitalWrite(DIR1,LOW); // Set Dir high  - Set chiều quay mức cao
digitalWrite(DIR2,HIGH); // Dir - chiều quay
//tiến thẳng
for(long i = 0; i < step1; i++) 
{
digitalWrite(STEP1,HIGH); // Output high - xung mức cao
digitalWrite(STEP2,HIGH); // Output high - xung mức cao
delay(kc/2);        
digitalWrite(STEP1,LOW); // Output low - xung mức thấp
digitalWrite(STEP2,LOW); // Output low - xung mức thấp
delay(kc/2);         // Trễ kc/2 ms
}
//ty le 1:3 trái:phải

for(long i = 1; i <= step1; i++) 

{ if (i%3==0)  digitalWrite(STEP1,HIGH); // Output high - xung mức cao
               digitalWrite(STEP2,HIGH); // Output high - xung mức cao
               delay(kc/2);        
  if (i%3==0)  digitalWrite(STEP1,LOW); // Output low - xung mức thấp
               digitalWrite(STEP2,LOW); // Output low - xung mức thấp
               delay(kc/2);         
}

}
