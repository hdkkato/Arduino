/**
to read serial data from TWE Lite in addition to ordinaly serial port, another serial port is need
so assign unused pin as virtual serial port
asigne pin 10 and pin 11 to TX and RX
**/
#include <SoftwareSerial.h>

int digitIn = 2;  // connect pin NO.2 to DO1 of TWE Lite
int digitOut = 3;

void setup() {
  pinMode(digitIn, INPUT);
  pinMode(digitOut, OUTPUT);
}

void loop() {
  if (digitalRead(digitIn) == HIGH) {
    digitalWrite(digitOut, HIGH);
  } else {
    digitalWrite(digitOut, LOW);
  }
}
