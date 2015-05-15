const int micPin = A0;
int base;
int val;
int buf[100];
int i;

void setup()
{
base = analogRead(micPin);
Serial.begin(9600);
}

void loop() {
val = analogRead(micPin);
unsigned long tStart = micros();

for (i = 0; i < 100; i++)
{
val = analogRead(micPin);
}

unsigned long tEnd = micros();
Serial.println(tEnd-tStart);
}
