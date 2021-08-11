#include "521.h"
#include <Wire.h>


int SLV_ADDR = 0x68;
uint8_t TEMP_MSR = 0x41;
uint8_t ACCEL_CONFIG = 0x1C;

int ledRed = 6;
int ledBlue = 5;
int ledGreen = 3;


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

void GY_521::print_accData(GY_521 &data)
{

  Serial.print("x = ");
  Serial.print(data.xData);
  Serial.print(",   y = ");
  Serial.print(data.yData);
  Serial.print(",   z =  "); 
  Serial.print(data.zData);
  Serial.println(" ");

  if(data.xData < 500)
    analogWrite(ledRed, 0);
  else if(data.xData >= 500 && data.xData < 3000)
      analogWrite(ledRed, 50);
  else if(data.xData >= 3000 && data.xData < 6000)
      analogWrite(ledRed, 100);
  else if(data.xData >= 6000 && data.xData < 9000)
      analogWrite(ledRed, 150);
  else if(data.xData >= 9000 && data.xData < 12000)
      analogWrite(ledRed, 200);
  else 
      analogWrite(ledRed, 400);
 
  if(data.yData < 100)
    analogWrite(ledBlue, 0);
  else if(data.yData >= 500 && data.yData < 3000)
      analogWrite(ledBlue, 25);
  else if(data.yData >= 3000 && data.yData < 6000)
      analogWrite(ledBlue, 75);
  else if(data.yData >= 6000 && data.yData < 9000)
      analogWrite(ledBlue, 100);
  else if(data.yData >= 9000 && data.yData < 19000)
      analogWrite(ledBlue, 150);
  else
      analogWrite(ledBlue, 0);

  if(data.zData < 500)
    analogWrite(ledGreen, 0);
  else if(data.zData >= 500 && data.zData < 3000)
      analogWrite(ledGreen, 50);
  else if(data.zData >= 3000 && data.zData < 6000)
      analogWrite(ledGreen, 100);
  else if(data.zData >= 6000 && data.zData < 9000)
      analogWrite(ledGreen, 150);
  else if(data.zData >= 9000 && data.zData < 12000)
      analogWrite(ledGreen, 200);
  else 
      analogWrite(ledGreen, 250);

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


void GY_521::selfTestAcc()
{

  uint8_t tempVal;
  uint8_t value1;
  uint8_t valueMixed;
  float trimX = 0;
  tempVal = readReg(ACCEL_CONFIG);
  
  // Enable Self Test bits: XA_ST=1, YA_ST =1, ZA_ST= 1
  // Set to 8g+-: AFS_SEL = 0x10
  tempVal = tempVal |= 0xF0;
  writeReg(ACCEL_CONFIG, tempVal);
  
  // Read Self test X register
  value1 = readReg(0xD);
  // Read Self Test A register
  valueMixed = readReg(0x10);

  // Combine [4-0] from test X reg with [0
  //Get bits 0-4
  value1 >>= 3;
  //combine with bits from XA_TEST
  value1 |= valueMixed & 0x30;
  value1 &= 0x1F;
  trimX = (4096 * 0.34) * pow ( (0.92/0.32)  , ((value1 - 1)/( pow(2,5) -2  )   )  );
    
  Serial.print("value 1 calibrated = ");
  Serial.print(value1);
  Serial.println(" ");
  Serial.print("trimX = ");
  Serial.print(trimX);
  Serial.println(" ");
  
  // Return to default setting
  tempVal = readReg(ACCEL_CONFIG);
  tempVal =0;
  writeReg(ACCEL_CONFIG, tempVal);
 
}

GY_521 GY_521::get_accData(GY_521& temp)
{
  temp;
  Wire.beginTransmission(SLV_ADDR);
  Wire.write(0x3B); // X high bytes
  Wire.endTransmission(false);
  Wire.requestFrom(SLV_ADDR, 6, true);
  temp.xData = Wire.read() <<8 | Wire.read();  
  temp.yData = Wire.read() <<8 | Wire.read();
  temp.zData = Wire.read() <<8 | Wire.read();
  delay(300);
  return temp;
}

GY_521 GY_521::getTempData(GY_521& temp)
{
  temp;
  temp.tempMsr = readReg(TEMP_MSR);
  temp.tempMsr =  (temp.tempMsr << 8) | readReg(TEMP_MSR+1);
  temp.tempMsr = (temp.tempMsr/340) + 36.53;
  temp.tempMsr = (temp.tempMsr *1.8) +32;
  Serial.print("temp = ");
  Serial.print(temp.tempMsr);
  Serial.println(" ");
  return temp;

}
