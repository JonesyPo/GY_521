
#include <Arduino.h>
class GY_521 {
  private:
    int16_t xData;
    int16_t yData;
    int16_t zData;
    
   public:
    GY_521 get_accData();
    void writeToReg();
    void selfTestAcc();
};


void writeReg(uint8_t reg, uint8_t val);


uint8_t readReg(uint8_t reg);





