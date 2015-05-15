const int micPin = A0;
const int potPin = A1;
int base;
int micVal;
int potVal;
unsigned long tStart;
unsigned long tEnd;

void setup() {
  base = analogRead(micPin);
  Serial.begin(9600);
}

void loop() {
  micVal = analogRead(micPin);
  if (abs(base - micVal) > 50)
  {
    tStart = micros();
    for (int i = 0; i<1000; i++) {
      micVal = analogRead(micPin);
      potVal = analogRead(potPin);
    }
    tEnd = micros();

    Serial.println(tEnd-tStart);
    Serial.println(potVal);
    Serial.println(micVal);
  }
}
