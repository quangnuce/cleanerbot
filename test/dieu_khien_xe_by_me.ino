#include "ros.h"
#include "geometry_msgs/Twist.h"
#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
float x;
float z;
long step1;
long kc;
ros::NodeHandle nh;
void velCallback(  const geometry_msgs::Twist& vel_cmd)
{
  x = vel_cmd.linear.x;
  z = vel_cmd.angular.z;
  if(x==0&&z!=0)
    x=0.2;
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);
void setup()

{
  nh.initNode();
  nh.subscribe(sub);
  pinMode(STEP1, OUTPUT); // Step
  pinMode(DIR1, OUTPUT); // Dir - chiều quay
  pinMode(STEP2, OUTPUT); // Step
  pinMode(DIR2, OUTPUT); // Dir - chiều quay

}
void tinh (float x)
{
  step1 = x / 0.00047124;
  kc = 1000000 / step1;//microsecond
  //in log
  char log_msg[50];
  char str_x[6];
  dtostrf(x, 4, 2, str_x);
  sprintf(log_msg,"x=%s,step1=%ld, kc=%ld",str_x,step1,kc);
  nh.loginfo(log_msg);
}
long buoc (float x, float z)
{
  long stepp = (x - tan(z) * 0.15) / 0.00047124;
  long tyle = step1 / stepp;
  
  return tyle;
}
void loop()
{
  nh.spinOnce();
  tinh (x);
  Serial.println(step1);
  Serial.print(kc);
  digitalWrite(DIR1, LOW); // Set Dir high  - Set chiều quay mức cao
  digitalWrite(DIR2, HIGH); // Dir - chiều quay
  long tyle=buoc(x, z);
  //tiến thẳng
  if (z == 0)
    for (long i = 0; i < step1; i++)
    {
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc / 2);
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc / 2);       // Trễ kc/2 ms
    }
  else if ( z > 0)
    // trái:phải(re trai)
    for (long i = 1; i <= step1; i++)

    { if (i % tyle == 0)  digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc / 2);
      if (i % tyle == 0)  digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc / 2);
    }
  else
    //rẽ phải
    for (long i = 1; i <= step1; i++)

    { if (i % tyle == 0)  digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc / 2);
      if (i % tyle == 0)  digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc / 2);
    }

}
