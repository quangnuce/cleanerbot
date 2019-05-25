#include "ros.h"
#include "geometry_msgs/Twist.h"
#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
#define VACUMM 32
float x;
float z;
long step1,step2;
long kc,kc2;
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
void tinh2 (float z)
{
  float abs_a=z>0?z:-z;
  step2 = abs_a*0.15 / 0.00047124;
  kc2 = 1000000 / step2;//microsecond
  kc2=kc2/2;
  //in log
  //char log_msg[50];
  //char str_x[6];
  //dtostrf(x, 4, 2, str_x);
  //sprintf(log_msg,"x=%s,step1=%ld, kc=%ld",str_x,step1,kc);
  //nh.loginfo(log_msg);
}
//long buoc (float x, float z)
//{
  //float abstan=tan(z)>0?tan(z):-tan(z);
  //long stepp = (x - abstan * 0.15) / 0.00047124;
  //long tyle = step1 / stepp;
  //char log_msg[50];
  //char str_x[6];
  //dtostrf(abstan, 4, 2, str_x);
  //sprintf(log_msg,"tang=%s,tyle=%ld,stepp=%ld,step1=%ld",str_x,tyle,stepp,step1);
  //nh.loginfo(log_msg);
  //if(tyle<0) tyle=2147483647;
 // long tyle=1;
  //float abs_a=z>0?z:-z;
 // if (abs_a<=0.5) tyle=2;
 // else if (abs_a<=0.75) tyle=3;
 // else if (abs_a<=1)tyle=4;
 // return tyle;
//}
void loop()
{
  nh.spinOnce();
  tinh (x);
  tinh2(z);
  //long tyle=buoc(x,z);
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
  float abs_a=z>0?z:-z;
  if (abs_a <= 0.264){
    
    char log_msg[50];

  sprintf(log_msg,"di thang step1=%ld, kc=%ld",step1,kc);
  nh.loginfo(log_msg);
    for (long i = 0; i < step1; i++)
    {
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc);
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc );       // Trễ kc ms
    }
  }
  else if ( z > 0) {
      // digitalWrite(STEP1, LOW);
    // trái:phải(re trai)
    char log_msg[50];

  sprintf(log_msg,"re trai step2=%ld, kc2=%ld",step2,kc2);
  nh.loginfo(log_msg);
    for (long i = 1; i <= step2; i++)

    { //if (i % tyle == 0)  
      // Output high - xung mức cao
      digitalWrite(STEP2, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc2 );
      //if (i % tyle == 0)  
      //digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      digitalWrite(STEP2, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc2 );
    }
    }
  else {
    //rẽ phải
     //digitalWrite(STEP2, LOW);
       char log_msg[50];

    sprintf(log_msg,"re phai step2=%ld, kc2=%ld",step2,kc2);
    nh.loginfo(log_msg);
    for (long i = 1; i <= step2; i++)

    { //if (i % tyle == 0)  
      // Output high - xung mức cao
      digitalWrite(STEP1, HIGH); // Output high - xung mức cao
      delayMicroseconds(kc2);
      
      digitalWrite(STEP1, LOW); // Output low - xung mức thấp
      delayMicroseconds(kc2);
    }
    }
   // if(x==0&&z==0)
     // delay(100);  
    
    //x=0;
    //z=0;

}
