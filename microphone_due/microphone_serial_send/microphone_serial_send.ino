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
const int bits = 12;                   // ADC resolution defaults to 10 bits
const float Vcc = 3.3;                 // voltage of power supply for microphone module
const float Vref = Vcc / 2;            // output from microphone module at quiet state

int base = Vref / Vcc * pow(2, bits);  // base signal at a quiet state
int sig;                               // signal from ADC in digital value
int threshold = 512;
int freq = 20000;                       // measuring frequence [Hz]
int sample;                            // sample number
int16_t buf[32768 + 4096] = {};        // buffer for signal
int cnt = 0;

DueTimer myTimer = DueTimer(4);

// handler for timer
void readMic() {
  buf[cnt] = analogRead(micPin);
  cnt++;
  if (cnt >= sample) {
    myTimer.stop();
  }
}

void setup() {
  analogReadResolution(bits);  // set ADC resolution
  Serial.begin(115200);
  sample = pow(2, 15);
}

void loop() {

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

  // wait until buffer get full,
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
    Serial.write("H");  //header
    Serial.write(highByte(buf[i]));  //high byte
    Serial.write(lowByte(buf[i]));  // low byte
  }
}

