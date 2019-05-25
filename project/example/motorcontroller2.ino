#include "ros.h"
#include "geometry_msgs/Twist.h"
#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
#define VACUMM 32
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
    x=0.1;
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
  pinMode(VACUMM, OUTPUT); // Dir - chiều quay

}
void tinh (float x)
{
  step1 = x / 0.00047124;
  kc = 1000000 / step1;//microsecond
  kc=kc/2;
  //in log
  char log_msg[50];
  char str_x[6];
  dtostrf(x, 4, 2, str_x);
  sprintf(log_msg,"x=%s,step1=%ld, kc=%ld",str_x,step1,kc);
  nh.loginfo(log_msg);
}
long buoc (float x, float z)
{
  //float abstan=tan(z)>0?tan(z):-tan(z);
  //long stepp = (x - abstan * 0.15) / 0.00047124;
  //long tyle = step1 / stepp;
  //char log_msg[50];
  //char str_x[6];
  //dtostrf(abstan, 4, 2, str_x);
  //sprintf(log_msg,"tang=%s,tyle=%ld,stepp=%ld,step1=%ld",str_x,tyle,stepp,step1);
  //nh.loginfo(log_msg);
  //if(tyle<0) tyle=2147483647;
  long tyle=1;
  float abs_a=z>0?z:-z;
  if (abs_a<=0.5) tyle=2;
  else if (abs_a<=0.75) tyle=3;
  else if (abs_a<=1)tyle=4;
  return tyle;
}
void loop()
{
  nh.spinOnce();
  tinh (x);
  long tyle=buoc(x,z);
  if(x==0&&z==0)
   digitalWrite(VACUMM, LOW);
  else
     digitalWrite(VACUMM, HIGH);
  Serial.println(step1);
  Serial.print(kc);
  digitalWrite(DIR1, LOW); // Set Dir high  - Set chiều quay mức cao
  digitalWrite(DIR2, HIGH); // Dir - chiều quay
  
  //long tyle=buoc(x, z);
  //char log_msg[50];
  //char str_x[6];
  //dtostrf(z, 4, 2, str_x);
  //sprintf(log_msg,"tyle=%ld,z=%s,kc=%ld ",tyle,str_x,kc);
  //nh.loginfo(log_msg);
  //tiến thẳng
  if (z == 0)
    for (long i = 0; i < step1; i++)
    {
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc);
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc );       // Trễ kc ms
    }
  else if ( z > 0)
    // trái:phải(re trai)
    for (long i = 1; i <= step1; i++)

    { if (i % tyle == 0)  digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc );
      if (i % tyle == 0)  digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc );
    }
  else
    //rẽ phải
    for (long i = 1; i <= step1; i++)

    { if (i % tyle == 0)  digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc);
      if (i % tyle == 0)  digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc);
    }
    if(x==0&&z==0){
      delay(100);  
    }
    //x=0;
    //z=0;

}
