#define STEP1 5
#define DIR1 4
#define STEP2 7
#define DIR2 6
long x;

void setup()

{

pinMode(STEP1,OUTPUT); // Step
pinMode(DIR1,OUTPUT); // Dir - chiều quay
pinMode(STEP2,OUTPUT); // Step
pinMode(DIR2,OUTPUT); // Dir - chiều quay

}

void loop()
{

digitalWrite(DIR1,HIGH); // Set Dir high  - Set chiều quay mức cao
pinMode(DIR2,LOW); // Dir - chiều quay
for(x = 0; x < 4096; x++) // lặp 4096 lần

{
digitalWrite(STEP1,HIGH); // Output high - xung mức cao
digitalWrite(STEP2,HIGH); // Output high - xung mức cao
delay(1);         // Trễ 1ms
digitalWrite(STEP1,LOW); // Output low - xung mức thấp
digitalWrite(STEP2,LOW); // Output low - xung mức thấp
delay(3);         // Trễ 3ms
}

// Serial.println("Pause");
delay(1000);               // dừng 1 giây

digitalWrite(DIR1,LOW); // Set Dir low - Set chiều quay mức thấp
digitalWrite(DIR2,HIGH); 
for(x = 0; x < 4096; x++) // lặp 4096 lần

{
digitalWrite(STEP1,HIGH); // Output high - xung mức cao
digitalWrite(STEP2,HIGH); // Output high - xung mức cao
delay(1);         // Trễ 1ms
digitalWrite(STEP1,LOW); // Output low - xung mức thấp
digitalWrite(STEP2,LOW); // Output low - xung mức thấp
delay(3);         // Trễ 3ms
}

// Serial.println("Pause");
delay(1000);               // dừng 1 giây
}
