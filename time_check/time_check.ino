unsigned long tStart;
unsigned long tEnd;
unsigned long tEllapsed;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("start");
  tStart = micros();
  for(int i=1;i<1000;i++)
  {
    Serial.println(100);
  }
  tEnd = micros();
  Serial.println("end");
  tEllapsed = tEnd - tStart;
  Serial.print("Ellapsed time: ");
  Serial.print(tEllapsed);
  Serial.println(" usec");
  delay(1000);
}
