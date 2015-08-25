#include <DueTimer.h>

int cnt = 0;
unsigned long tStart;
unsigned long tEnd;
uint16_t buf[10000] = {};

void myHandler() {
  buf[cnt] = analogRead(A0);
  cnt ++;
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);
  Timer4.attachInterrupt(myHandler).start(10);
tStart = micros();
}

void loop() {
  if (cnt == 10000) {
    Timer4.stop();
    tEnd = micros();
    Serial.println(tEnd-tStart);
    cnt = 0;
    delay(100);
    for (int i = 0; i < 10000; i++) {
      Serial.println(buf[i]);

    }
    
      delay(100);
  }
}

