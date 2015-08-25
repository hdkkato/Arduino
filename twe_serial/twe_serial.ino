/**
to read serial data from TWE Lite in addition to ordinaly serial port, another serial port is need
so assign unused pin as virtual serial port
asigne pin 10 and pin 11 to TX and RX
**/
#include <SoftwareSerial.h>

char recv;
SoftwareSerial twe(10, 11);  // use virtual serial port TX:11 RX:10

void setup() {
  Serial.begin(9600);  // connect to serial port 
  twe.begin(115200);  // connect to TX and RX port of TWE Lite
}

void loop() {
  if (twe.available() > 0) {
      recv = twe.read();  // read from RX of TWE Lite
      Serial.write(recv);  // print on display
}
}
