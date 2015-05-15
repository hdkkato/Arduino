#include <Wire.h>
#include <Streaming.h>

#define ADDR_ADC121             0x55

#define V_REF 3.00

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

unsigned int getData;
float analogVal=0;         // convert
void init_adc()
{
  Wire.beginTransmission(ADDR_ADC121);        // transmit to device
  Wire.write(REG_ADDR_CONFIG);                // Configuration Register
  Wire.write(0x20);
  Wire.endTransmission();  
}

unsigned int read_adc()     //unsigned int *data
{
   
   
    Wire.beginTransmission(ADDR_ADC121);        // transmit to device
    Wire.write(REG_ADDR_RESULT);                // get result
    Wire.endTransmission();

    Wire.requestFrom(ADDR_ADC121, 2);           // request 2byte from device
    delay(1);
    if(Wire.available()<=2)
    {
      getData = (Wire.read()&0x0f)<<8;
      Serial.print("high: ");
      Serial.print(getData);
      getData |= Wire.read();
      Serial.print("low: ");
      Serial.println(getData);
    }
    return getData;
//    Serial.write("H");  //header
//    Serial.write(highByte(getData)); //high byte
//    Serial.write(lowByte(getData));// low byte
    delay(5);
}
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  init_adc();
}

void loop()
{//adcRead);
unsigned int data = read_adc();
// Serial.println(data, DEC);
   delay(50);
}
