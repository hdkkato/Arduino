#include <Servo.h>
Servo myServo;

int const potPin = A0;
int potVal;
int angle;

void setup() {
  myServo.attach(9);
  //myServo.write(90);
  Serial.begin(9600);
  delay(1000);
  
}

void loop() {
//  potVal = analogRead(potPin);
//  Serial.print("PotVal: ");
//  Serial.print(potVal);
//  angle = map(potVal, 0, 1023, 0, 179);
//  Serial.print(", angle: ");
//  Serial.println(angle);
//  myServo.write(0);
  
  delay(2000);
  myServo.write(0);
  delay(2000);
  myServo.write(90);
  delay(2000);
  myServo.write(180);
  delay(2000);
  myServo.write(90);
  Serial.println("test");
}
