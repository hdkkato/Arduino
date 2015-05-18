/*
Electret Microphone Amplifier - MAX4466 with Adjustable Gain
https://www.adafruit.com/products/1063

Electret Capsule Microphone
supply curent 0.5 mA max
Vout: o to Vcc
Vout in the state of silence: 1.0V
base signal at Vcc = 3.3 V: 1.0 / Vcc * 1024 = 310

MAX4466 Low-Noise Microphone Amp

*/

const int micPin = A0;  // analog pin
int base;               // base signal in a state of silence
int val;                // digital value of measured voltage
int buf[600];           // buffer for val
int i;                  // iterator

void setup() {
  base = 310;
  Serial.begin(9600);
}

void loop() {
  val = analogRead(micPin);
  if (abs(base - val) > 100)
  {
    for (i = 0; i < 600; i++) {
      val = analogRead(micPin);
      buf[i] = val;
    }
    for (i = 0; i < 600; i++)
    {
      Serial.println(buf[i]);
    }
  }
}
