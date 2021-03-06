#include <Arduino.h>
class GY_521 {
  private:
    int16_t xData;
    int16_t yData;
    int16_t zData;
    int16_t tempMsr;
    
   public:
    GY_521 get_accData(GY_521& temp);
    GY_521 getTempData(GY_521& temp);
    void print_accData(GY_521& data);
    void writeToReg();
    void selfTestAcc();

};

void writeReg(uint8_t reg, uint8_t val);

uint8_t readReg(uint8_t reg);
