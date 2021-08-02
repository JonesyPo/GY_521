#include "521.h"
#include <Wire.h>

GY_521 reading;

void setup() {
  Serial.begin(115200);
  delay(2000);
  //Joining I2C bus as master so no address is passed in
  Wire.begin();  
  Wire.beginTransmission(0X68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  reading.selfTestAcc();
}

void loop() {
  reading.get_accData(reading);
  reading.print_accData(reading);
}
