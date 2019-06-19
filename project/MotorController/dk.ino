#include "ros.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
#define INA 3
#define INB 4
#define INC 5
#define IND 6
#define ENA 8
#define ENB 9
float HR;
float Va,Vb,V,V0;
ros::NodeHandle nh;
void velCallback(  const geometry_msgs::Twist& vel_cmd)
{
     V0 = vel_cmd.linear.x/0.15; 
     HR = vel_cmd.angular.z;   
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);
void setup()
{
 nh.initNode();
 nh.subscribe(sub);
 pinMode(INA,OUTPUT);
 pinMode(INB,OUTPUT);
 pinMode(INC,OUTPUT);
 pinMode(IND,OUTPUT);
}
void loop()
{  
nh.spinOnce();
int Vmin=80;
   float absHR=HR<0?-HR:HR;
  if (absHR < 0.05) {
    Va = Vb = V = V0 * (255 - Vmin) + Vmin;
  }
  if (absHR > 0.05) {
    if (HR > 0) {
      Va = V0 * (225 - Vmin) + Vmin;
      Vb = Va * (1 - absHR);
    }
    else if (HR < 0) {
      Vb = V0 * (225 - Vmin) + Vmin;
      Va = Vb * (1 - absHR);
    }
  }
  if (absHR < 0.05) {

    digitalWrite(INA, HIGH);
    digitalWrite(INC, HIGH);
    digitalWrite(INB, LOW);
    digitalWrite(IND, LOW);
    analogWrite(ENA, Va);
    analogWrite(ENB, Vb);
    delay(1000);
  }
   if (absHR > 0.05) {

    if (HR > 0) {
      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(ENA, Va);
      analogWrite(ENB, Vb);
      delay (1000);
    }
    else if (HR < 0) {

      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(ENA, Va);
      analogWrite(ENB, Vb);
      delay (1000);
    }
    Serial.print("Va: ");Serial.println(Va);
  Serial.print("Vb: ");Serial.println(Vb);
   }
   char log_msg[50];
  char str_va[6];
  char str_vb[6];
  dtostrf(Va, 4, 2, str_va);
  dtostrf(Vb, 4, 2, str_vb);
  sprintf(log_msg,"Va=%s,Vb=%s",str_va,str_vb);
  nh.loginfo(log_msg);
}
