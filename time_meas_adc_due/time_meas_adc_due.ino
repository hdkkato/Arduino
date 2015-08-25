/*
Description
time measurement of Arduino due ADC
*/

const int analogPin = A0;
const int bits = 12;      // resolution of adc dafaults to 10 bit

unsigned long tStart;
unsigned long tEnd;
unsigned long tEllapsed;

int buf[1000];

void setup() {
  analogReadResolution(bits);
  Serial.begin(9600);
}

void loop() {
  Serial.println("start");
  tStart = micros();
  for(int i=1;i<1000;i++)
  {
    buf[i] = analogRead(analogPin);
    delayMicroseconds(7);
  }
  tEnd = micros();
  Serial.println("end");
  tEllapsed = tEnd - tStart;
  Serial.print("Ellapsed time: ");
  Serial.print(tEllapsed);
  Serial.println(" usec");
  delay(1000);
}
