/*
Electret Microphone Amplifier - MAX4466 with Adjustable Gain
https://www.adafruit.com/products/1063

Electret Capsule Microphone
supply curent 0.5 mA max
Vcc: 3 V (standard) to 10 V (max)
Vout: o to Vcc
Vout in the state of silence: Vcc/2
base signal at Vcc = 3.3 V: 3.3/2 5 * 1024 = 337

MAX4466 Low-Noise Microphone Amp

*/

const int micPin = A0;  // analog pin
int base = 337;         // base signal in a state of silence
int threshold = 128;    // 
int sampleNum = 512;        // 
int10_t buf[512];      // buffer for val


void setup() {
  Serial.begin(9600);
}

void loop() {
  if (abs(base - analogRead(micPin)) > threshold)
  {
    for (int i = 0; i < sampleNum; i++) {
      buf[i] = analogRead(micPin);
    }
    for (int i = 0; i < sampleNum; i++)
    {
      Serial.println(buf[i]);
    }
  }
}
