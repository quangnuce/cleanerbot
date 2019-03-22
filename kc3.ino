#include <ros.h>
#include <ros/time.h>
// Sử dụng thư viện Servo

#include <Servo.h>
#define sensor A0
#include <ros.h>
#include <sensor_msgs/Range.h>

sensor_msgs::Range range_msg ;
ros::NodeHandle  nh;
ros::Publisher pub_range("range_data", &range_msg);

// Khai báo chân Trig và Echo
const int trigPin = 10;
const int echoPin = 11;
// Khai báo biến thời gian và khoảng cách
long duration;
float distance;
unsigned long range_timer;
char frameid[] = "/ir_ranger";
Servo myServo; // Tạo đối tượng Servo để điều khiển Servo
void setup() {
  nh.initNode();
  nh.advertise(pub_range);
  pinMode(trigPin, OUTPUT); // Set chân trig là chân OUTPUT
  pinMode(echoPin, INPUT); // Set chân echo INPUT
  Serial.begin(9600);
  myServo.attach(9); // Chân Servo là chân 12
  
  
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.01;//i: giá trị góc
  range_msg.min_range = 0.03;
  range_msg.max_range = 0.4;
  

}
void loop() {
  // Quay Servo từ 15 độ - 165 độ
  for (int i = 15; i < 165; i++) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();// Gọi hàm tính khoảng cách
   
    Serial.println(i);
    Serial.println("r");
    Serial.println(distance);
  range_msg.field_of_view = i;
     range_msg.range = distance;
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
    
  }

  
   
    
 // nh.spinOnce();
  
 
  //delay(100);
  // Sau khi quay xong Servo từ 15 - 165 độ....quay ngược lại từ 165 độ về 15 độ, sau đó gửi tương tự các giá trị như bên trên
  for (int i = 164; i > 15; i--) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();
    Serial.println(i);
    Serial.println("r");
    Serial.println(distance);

    range_msg.field_of_view = i;
     range_msg.range = distance;//kc o được
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
  
  }
 nh.spinOnce();
  //delay(1000);
}
// Hàm tính khoảng cách
float calculateDistance() {
  float volts = analogRead(sensor);  // value from sensor * (5/1024)
  float distance = 10650.08 * pow (volts, -0.935) - 10; // worked out from datasheet graph
  distance = distance / 100;
  // if (distance <= 180){
  //   Serial.println(distance);   // print the distance
  return distance;

}
