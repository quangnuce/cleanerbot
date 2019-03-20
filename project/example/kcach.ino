#include <ros.h>

// Sử dụng thư viện Servo

#include <Servo.h>
#define sensor A0
// Khai báo chân Trig và Echo
const int trigPin = 10;
const int echoPin = 11;
// Khai báo biến thời gian và khoảng cách
long duration;
int distance;
Servo myServo; // Tạo đối tượng Servo để điều khiển Servo
void setup() {
    pinMode(trigPin, OUTPUT); // Set chân trig là chân OUTPUT
    pinMode(echoPin, INPUT); // Set chân echo INPUT
    Serial.begin(9600);
    myServo.attach(9); // Chân Servo là chân 12
}
void loop() {
    // Quay Servo từ 15 độ - 165 độ
    for(int i=15;i<=165;i++){
        myServo.write(i);
        delay(10);
        distance = calculateDistance();// Gọi hàm tính khoảng cách
        Serial.print(i); // Gửi giá trị i ( góc quay của Servo) đến Serial Port
        Serial.print(","); // Gửi ","
        Serial.print(distance); // Tiếp theo gửi các giá trị khoảng cách tới Serial Port
        Serial.print("."); // Gửi dấu "."
    }
    // Sau khi quay xong Servo từ 15 - 165 độ....quay ngược lại từ 165 độ về 15 độ, sau đó gửi tương tự các giá trị như bên trên
    for(int i=165;i>15;i--){
        myServo.write(i);
        delay(10);
        distance = calculateDistance();
        Serial.print(i);
        Serial.print(",");
        Serial.print(distance);
        Serial.print(".");
    }
}
// Hàm tính khoảng cách  
int calculateDistance(){
    float volts = analogRead(sensor);  // value from sensor * (5/1024)
  int distance =10650.08 * pow (volts, -0.935) - 10; // worked out from datasheet graph
 // if (distance <= 180){
 //   Serial.println(distance);   // print the distance
    return distance;
}
