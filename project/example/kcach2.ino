#include <ros.h>
#include <ros/time.h>
// Sử dụng thư viện Servo

#include <Servo.h>
#define sensor A0
#include <ros.h>
#include <sensor_msgs/LaserScan.h>
sensor_msgs::LaserScan scan;
ros::NodeHandle  nh;
ros::Publisher chatter("scan", &scan);

// Khai báo chân Trig và Echo
const int trigPin = 10;
const int echoPin = 11;
// Khai báo biến thời gian và khoảng cách
long duration;
float distance;
float a[150];
unsigned int num_readings = 150;
double laser_frequency = 1000;
float* ranges;
//double* intensities;
Servo myServo; // Tạo đối tượng Servo để điều khiển Servo
void setup() {
  nh.initNode();
  nh.advertise(chatter);
  pinMode(trigPin, OUTPUT); // Set chân trig là chân OUTPUT
  pinMode(echoPin, INPUT); // Set chân echo INPUT
  Serial.begin(9600);
  myServo.attach(9); // Chân Servo là chân 12
  ranges = new float[150];
  //  intensities=new double[150];
}
void loop() {
  // Quay Servo từ 15 độ - 165 độ
  for (int i = 15; i < 165; i++) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();// Gọi hàm tính khoảng cách
    a[i - 15] = distance;
    Serial.println(i);
    Serial.println("r");
    Serial.println(a[i - 15]);
  }
  Serial.println("hello");
  //ros::Time scan_time = ros::Time::now();

  scan.header.stamp = nh.now();;
  scan.header.frame_id = "laser_frame";
  scan.angle_min = 0.26;
  scan.angle_max = 2.88;
  scan.angle_increment = 0.017;
  scan.time_increment = 0.05;
  scan.range_min = 0.2;
  scan.range_max = 1.8;

  //scan.ranges.resize=150;
  //scan.intensities.resize=150;
  for (unsigned int i = 0; i < num_readings; ++i) {
    scan.ranges[i] = a[i];
    scan.intensities[i] = 0;

  }
  chatter.publish( &scan );
  //nh.spinOnce();
  delay(1000);
  // Sau khi quay xong Servo từ 15 - 165 độ....quay ngược lại từ 165 độ về 15 độ, sau đó gửi tương tự các giá trị như bên trên
  for (int i = 164; i > 15; i--) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();
    a[i - 15] = distance;
    Serial.println(i);
    Serial.println("r");
    Serial.println(a[i - 15]);
  }
  for (unsigned int i = 0; i < num_readings; ++i) {
    scan.ranges[i] = a[i];
    scan.intensities[i] = 0;

  }
  chatter.publish( &scan );
  nh.spinOnce();
  delay(1000);
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
