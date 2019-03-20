#define BLYNK_PRINT Serial
#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include <math.h>
 #define INA 3
 #define INB 4
 #define INC 5
 #define IND 6
 #define ENA 8
 #define ENB 9
 #define BLYNK_USE_DIRECT_CONNECT.
 const double pi=acos(-1.0);
char auth[] = "31fd12d8bcce434497b18884da7b6afc";

SoftwareSerial SerialBLE(10, 11); // RX, TX
String dk;
BLYNK_WRITE(V1) 
{
  double goc;
  int x = param[0].asInt();
  int y = param[1].asInt();
  goc=atan2(y-128,x-128);
 // if (y>=128&&x<128) goc=pi-goc;
 // if (y<=128&&x<128) goc=goc+pi;
 // if (y<=128&&x>128) goc=2*pi-goc;
  Serial.println(goc);
  if (x==y&&y==128) dk="dung"; else 
  if (pi/4<=goc&&goc<=pi*3/4) dk="len";
  else if (-pi/4>=goc&&goc>=-3*pi/4) dk="lui";
  else if (pi*3/4<goc||goc<-3*pi/4) dk="trai";
  else if (goc<pi/4||goc>-pi/4) dk="phai";
 
  // Do something with x and y
 // Serial.print("X = hhahahahahahahahahahahahaha ");
  Serial.print(dk);
 // Serial.println(pi);
  //Serial.println(y);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(INC, OUTPUT);
  pinMode(IND, OUTPUT);
  Serial.println("Waiting for connections...");
}

void loop()
{
  Blynk.run();
  analogWrite(ENA,255);
  analogWrite(ENB,255);
 // Serial.println(dk);
   if(dk == "lui")
  {
    //Serial.print("nguyentranhai");
    digitalWrite(INA, HIGH);
    digitalWrite(INC, HIGH);
    digitalWrite(INB,LOW);
    digitalWrite(IND,LOW);
    delay(100);
  }

  else if(dk == "len")
  {
    digitalWrite(INA, LOW);
    digitalWrite(INC, LOW);
    digitalWrite(INB, HIGH);
    digitalWrite(IND,HIGH);
    delay(100);
  }

  else if (dk == "trai")
  {
    digitalWrite (INB,HIGH);
    digitalWrite (INC,LOW);
    digitalWrite (INA,LOW);
    digitalWrite (IND,LOW);
    delay (100);
   
  }

 else if ( dk == "phai")
 {
   digitalWrite (INA, LOW);
   digitalWrite (IND, HIGH);
   digitalWrite (INB, LOW);
   digitalWrite (INC, LOW);
   delay (100);
 }

 else // if (dk == "dung")
 {
   digitalWrite (INA, LOW);
   digitalWrite (INB, LOW);
   digitalWrite (INC, LOW);
   digitalWrite (IND, LOW);
   delay (100);
 }

//dk="";
}
