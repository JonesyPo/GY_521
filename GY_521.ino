
#include <Wire.h>

int SLV_ADDR = 0x68;
uint8_t ACCEL_CONFIG = 0x1C;

class GY_521 {
  private:
    int16_t xData;
    int16_t yData;
    int16_t zData;
    
   public:
    void get_accData();
    void writeToReg();
};

GY_521 reading;

void writeReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission(true);
  Serial.print("Wrote ");
  Serial.print(val, HEX);
  Serial.print(" to register ");
  Serial.print(reg, HEX);
  Serial.println(" ");
}

uint8_t readReg(uint8_t reg)
{
  uint8_t tempReadVal = 0;
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(SLV_ADDR, 1, true);
  tempReadVal = Wire.read();
  Serial.print("Value at register ");
  Serial.print(reg, HEX);
  Serial.print(" = ");
  Serial.print(tempReadVal, HEX);
  Serial.println(" ");
  Wire.endTransmission(true);
  return tempReadVal;
}


void selfTestAcc()
{
  //set full-scale range to +-8g/+-1000 deg/s
  uint8_t tempVal;
  tempVal = readReg(ACCEL_CONFIG);
  tempVal = tempVal |= 0x10;
  writeReg(ACCEL_CONFIG, tempVal);


  // Do test here


  // Return to default setting
  tempVal = readReg(ACCEL_CONFIG);
  tempVal &= 0xE7;
  writeReg(ACCEL_CONFIG, tempVal);
}

void GY_521::get_accData()
{
  GY_521 temp;
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(0x3B); // X high bytes
  Wire.endTransmission(false);
  Wire.requestFrom(SLV_ADDR, 6, true);
  temp.xData = Wire.read() <<8 | Wire.read();  
  temp.yData = Wire.read() <<8 | Wire.read();
  temp.zData = Wire.read() <<8 | Wire.read();
  Serial.print("x = ");
  Serial.print(temp.xData);
  Serial.print(",   y = ");
  Serial.print(temp.yData);
  Serial.print(",   z =  "); 
  Serial.print(temp.zData);
  Serial.println(" ");
  delay(1000);
  
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  //Joining I2C bus as master so no address is passed in
  Wire.begin();  
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  selfTestAcc();
}

void loop() {
  reading.get_accData();
}
