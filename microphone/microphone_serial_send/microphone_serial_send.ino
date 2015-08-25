/*
Description
sketch for Arduino UNO

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


Signal
Baseline(Vcc = 3.3V): 3.3/2 = 1.65 V
->1.65/5 * 1024 = 337 

threshold:0.5 V
-> 0.5/5 *1024 = 102 -> 128
*/

const int micPin = A0;  // analog pin
int base = 337;         // base signal in a state of silence
int threshold = 128;    // 
int sampleNum = 512;        // 
int16_t buf[512];      // buffer for val


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
      /*
      processing cannot receive more than 2 byte data at once.
      send header + high 1 byte + low 1 byte data.
      */
      Serial.write("H");  //header
      Serial.write(highByte(buf[i])); //high byte
      Serial.write(lowByte(buf[i]));// low byte
      delay(10);
    }
  }
}
