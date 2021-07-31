
#include <Wire.h>

int SLV_ADDR = 0x68;

class SensorData {
  private:
    int16_t xData;
    int16_t yData;
    int16_t zData;
   public:
    void get_accData();
};

SensorData reading;

void SensorData::get_accData()
{
  SensorData temp;
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(0x3B); // X high bytes
  Wire.endTransmission(false);
  Wire.requestFrom(SLV_ADDR, 6, true);
  temp.xData = Wire.read() <<8 | Wire.read();  
  temp.yData = Wire.read() <<8 | Wire.read();
  temp.zData = Wire.read() <<8 | Wire.read();
  Serial.print("x =");
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
}

void loop() {
  reading.get_accData();
}
