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

const int micPin = A0;                // analog pin
const int bits = 12;                  // ADC resolution defaults to 10 bits
const float Vcc = 3.3;                // voltage of power supply for microphone module
const float Vref = Vcc/2;             // output from microphone module at quiet state

int base = Vref/Vcc * pow(2, bits);   // base signal at a quiet state
int sig;                              // signal from ADC in digital value
int sample = 512;
int threshold = 128;                  // int digit
uint16_t buf[512] = {};               // buffer, type is unified to 16 bit

void setup() {
  analogReadResolution(bits);  // change ADC resolution
  Serial.begin(9600);
}

void loop() {
  sig = analogRead(micPin);
  if (abs(base - sig) > threshold){
    for (int i = 0; i < sample; i++) {
      buf[i] = analogRead(micPin);
      delayMicroseconds(7);  // analogRead takes about 3 usec
    }
    for (int i = 0; i < sample; i++)
    {
      Serial.println(buf[i]);
    }
  }
}
