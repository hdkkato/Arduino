const int micPin = A0;
int base;
int val;
int buf[600];
int i;

void setup() {
  base = analogRead(micPin);
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
      /*
      processing cannot receive more than 2 byte data at once.
      send header + high 1 byte + low 1 byte data.
      */
      Serial.write("H");  //header
      Serial.write(highByte(buf[i])); //high byte
      Serial.write(lowByte(buf[i]));// low byte

    }
  }
}
