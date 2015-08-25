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
*/

#include <DueTimer.h>

const int micPin = A0;     // analog pin
const int bits = 12;       // ADC resolution defaults to 10 bits
const float Vcc = 3.3;     // voltage of power supply for microphone module
const float Vref = Vcc / 2;// output from microphone module at quiet state

int base;                  // base signal at a quiet state
int sig;                   // signal from ADC in digital value
int freq = 20000;          // measuring frequence [Hz]
int sample = 40000;
int threshold = 300;
int16_t buf[40000] = {};   // buffer for signal
int cnt = 0;

void readMic() {
  buf[cnt] = analogRead(micPin);
  cnt ++;
}

void setup() {
  analogReadResolution(bits);  // change ADC resolution
  base = Vref / Vcc * pow(2, bits);
  Serial.begin(9600);
}

void loop() {
  sig = analogRead(micPin);
  if (abs(base - sig) > threshold)
  {
    // call readMic 20k times a second
    Timer4.attachInterrupt(readMic).setFrequency(freq).start();
  }
  if (cnt == sample) {
    Timer4.stop();
    cnt = 0;
    for (int i = 0; i < sample; i++)
    {
      Serial.println(buf[i]);
      delay(20);
    }
  }
}
