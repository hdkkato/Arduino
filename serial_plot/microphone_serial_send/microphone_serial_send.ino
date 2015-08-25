/*
Description
sketch for Arduino DUE

Warning:
Unlike other Arduino boards, the Arduino Due board runs at 3.3V.
The maximum voltage that the I/O pins can tolerate is 3.3V.
Providing higher voltages, like 5V to an I/O pin could damage the board.
(from http://www.arduino.cc/en/Main/arduinoBoardDue)

Electret Microphone Amplifier - MAX4466 with Adjustable Gain
https://www.adafruit.com/products/1063
base voltage:  Vcc/2 = 1.65V (Vcc = 3.3V)
Vout:          0.01V/3.29V (Vcc = 3.3V)
gain:          x23/x123

Electret Capsule Microphone
supply curent: 0.5mA max
Vcc:           3 V (standard) to 10 V (max)
Sensitivity:   -44dB (0dB = 1V/Pa) -> 6.3mV/Pa

MAX4466 Low-Noise Microphone Amp
Vcc:
bias voltage:  Vcc/2
Vout:          0.01V/3.29V

2015/05/21 HK copied from microphone_uno.ino
2015/05/21 HK fixed
2015/05/22 HK fixed
2015/06/05 HK fixed
*/

#include <DueTimer.h>

const int micPin = A0;                 // analog pin
const int resolution = 12;             // ADC resolution defaults to 10 bits
const float Vcc = 3.3;                 // voltage of power supply for microphone module
const float Vref = Vcc / 2;            // output from microphone module at quiet state

int base;                              // base signal at a quiet state
int sig;                               // signal from ADC in digital value
int threshold;
int freq;                              // measuring frequence [Hz]
int sample;                            // sample number
int cnt;
int flag;
int16_t buf[32768 + 4096] = {};        // buffer for signal

DueTimer myTimer = DueTimer(4);


void setup() {
  base = Vref / Vcc * pow(2, resolution);
  threshold = 512;
  freq = 20000;
  cnt = 0;
  sample = pow(2, 15);  
  
  analogReadResolution(resolution);  // set ADC resolution
  Serial.begin(115200);
}


// built-in function: serial interruption
// this function runs between each time loop() runs
void serialEvent() {
  if (Serial.available() > 0) {
    char recv = Serial.read();
    if (recv == 't') {  // triger
      flag = 1;
    } else if (recv == 'r') { // running
      flag = 2;
    } else flag = 0;
  }
}


void loop() {
  // switch measureing mode due to flag
  switch (flag) {
    case 1:  // triger mode
      readMicByTriger();
      break;
    case 2:  // free running mode
      readMicRunning();
      break;
    default:
      delay(1);
      break;
  }
}

void readMicByTriger() {
  // until abs(sig - base) exceed threshold, read analog value of micPin
  // then initialize timer and break while loop
  while (true) {
    sig = analogRead(micPin);
    if (abs(sig - base) > threshold) {
      // call readMic at freq Hz
      myTimer.attachInterrupt(readMic);
      myTimer.setFrequency(freq);
      myTimer.start();
      break;
    }
  }
  // wait until buffer gets full,
  while (true) {
    delay(1);
    if (cnt >= sample) {
      cnt = 0;
      break;
    }
  }
  // send header character "H" and 2 byte signal
  // such as "H1927H2158H2272..."
  // write 1 byte data each time
  // because processing receives only 1 byte at one time
  for (int i = 0; i < sample; i++) {
    //Serial.write("H");
    Serial.println(buf[i]);
//    Serial.write("H");  // header
//    Serial.write(highByte(buf[i])); // high byte
//    Serial.write(lowByte(buf[i]));  // low byte
  }
  while (Serial.available() == 0){
  Serial.write(byte(0x03));  // ETX (End of TeXt)
  }
}


void readMicRunning() {
  // call readMic at freq Hz
  myTimer.attachInterrupt(readMic);
  myTimer.setFrequency(freq);
  myTimer.start();

  // wait until buffer gets full,
  while (true) {
    delay(1);
    if (cnt >= sample) {
      cnt = 0;
      break;
    }
  }
  // send header character "H" and 2 byte signal
  // such as "H1927H2158H2272..."
  // write 1 byte data each time
  // because serial communication send only 1 byte at one time
  for (int i = 0; i < sample; i++) {
    //Serial.write("H");
    Serial.println(buf[i]);
//    Serial.write("H");  // header
//    Serial.write(highByte(buf[i])); // high byte
//    Serial.write(lowByte(buf[i]));  // low byte
  }
  while (Serial.available() == 0){
  Serial.write(byte(0x03));  // ETX (End of TeXt)
  }
}


void readMic() {
  // handler for timer
  buf[cnt] = analogRead(micPin);
  cnt++;
  if (cnt >= sample) {
    myTimer.stop();
  }
}
