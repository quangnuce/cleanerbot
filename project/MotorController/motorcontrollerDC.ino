#include "ros.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
#define INA 3
#define INB 4
#define INC 5
#define IND 6
#define ENA 8
#define ENB 9
#define VACUMM 32
float HR;
float Va, Vb, V;
ros::NodeHandle nh;
unsigned long timestamp_last_message;
void velCallback(  const geometry_msgs::Twist& vel_cmd)
{
  if(vel_cmd.linear.x!=0)
    V = vel_cmd.linear.x / 0.15 * 1+ V * 0;
   else V=0;
  HR = vel_cmd.angular.z * 1 + HR * 0;
   timestamp_last_message = millis();
 
  char str_v0[6];
  char str_hr[6];
  char log_msg[50];
  dtostrf(V, 4, 2, str_v0);
  dtostrf(HR, 4, 2, str_hr);
  sprintf(log_msg, "before V=%s,HR=%s", str_v0, str_hr);
  nh.loginfo(log_msg);
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);
void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(INC, OUTPUT);
  pinMode(IND, OUTPUT);
  pinMode(VACUMM, OUTPUT); // Dir - chiều quay
  V = 0;
  timestamp_last_message = millis();

}
void loop()
{
  nh.spinOnce();
  int Vmin = 40;
  float absHR = HR < 0 ? -HR : HR;
  int next_move = 0; //0: dung yen, 1: thang, 2: re (tien), 3: re (lui)
  if (absHR == 0 && V == 0) {//dung yen
    V = 0;
    Va = Vb = 0;
    next_move = 0;
  }
 

  if (absHR <= 0.05 && V != 0) {//di thang
    Va = Vb  = V * (255 - Vmin) + Vmin;
    next_move = 1;
  }

  if (absHR > 0.05) {
    if (V == 0) {
      V = 0.3;
      next_move = 3;
    }
    else next_move = 2;
    if (HR > 0) {
      Va = V * (225 - Vmin) + Vmin;
      Vb = Va * (1 - absHR);   
    }
    else if (HR < 0) {
      Vb = V * (225 - Vmin) + Vmin;
      Va = Vb * (1 - absHR);
    }
  }
  //dk dong co
  if (next_move == 0) { //dung yen
    nh.loginfo("dung yen"); 
    digitalWrite(VACUMM, LOW);
    dungyen(1000);
  }
  else{
     digitalWrite(VACUMM, HIGH);
    
  }
  if (next_move == 1) { //di thang
    nh.loginfo("di thang");
    tien(Va,Vb,1000); 
   
  }
  if (next_move == 2) {
     nh.loginfo("re");
    if (HR != 0) {
      tien(Va,Vb,1000);     
    }
  }
  if (next_move == 3) {
     nh.loginfo("lui");
    if (HR != 0) {
      lui(Va,Vb,1000);
    }
  }



  Serial.print("Va: "); Serial.println(Va);
  Serial.print("Vb: "); Serial.println(Vb);

  char log_msg[50];
  char str_va[6];
  char str_vb[6];
  char str_v0[6];
  char str_hr[6];
  dtostrf(Va, 4, 2, str_va);
  dtostrf(Vb, 4, 2, str_vb);
  dtostrf(V, 4, 2, str_v0);
  dtostrf(HR, 4, 2, str_hr);
  sprintf(log_msg, "Va=%s,Vb=%s,V=%s,HR=%s", str_va, str_vb, str_v0, str_hr);
  nh.loginfo(log_msg);
  //decrease after loop
  unsigned long current_millis = millis();
  if ((current_millis - timestamp_last_message) > 1500) {
    HR = HR * 0.5;
    V = V * 0.7;
    if (V <= 0.01)V = 0;
    if (absHR <= 0.01)HR = 0;
  }
}
void dungyen(int duration){
     digitalWrite(INA, LOW);
    digitalWrite(INC, LOW);
    digitalWrite(INB, LOW);
    digitalWrite(IND, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    delay(duration);
}
void tien(float Vt,float Vp,int duration){
    digitalWrite(INA, HIGH);
    digitalWrite(INC, HIGH);
    digitalWrite(INB, LOW);
    digitalWrite(IND, LOW);
    analogWrite(ENA, Vt);
    analogWrite(ENB, Vp);
    delay(duration);
}
void lui(float Vt,float Vp,int duration){
    digitalWrite (INC, LOW);
    digitalWrite (INA, LOW);
    digitalWrite (INB, HIGH);
    digitalWrite (IND, HIGH);
    analogWrite(ENA, Vp);
    analogWrite(ENB, Vt);
    delay (duration);
}
