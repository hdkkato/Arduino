/*

*/

const int analogPin = A0;
const int bits = 12;

unsigned long tStart;
unsigned long tEnd;
unsigned long tEllapsed;


void setup() {
  analogReadResolution(bits);
  Serial.begin(9600);
}

void loop() {
  Serial.println("start");
  tStart = micros();
  for(int i=1;i<1000;i++)
  {
    analogRead(analogPin);
  }
  tEnd = micros();
  Serial.println("end");
  tEllapsed = tEnd - tStart;
  Serial.print("Ellapsed time: ");
  Serial.print(tEllapsed);
  Serial.println(" usec");
  delay(1000);
}
