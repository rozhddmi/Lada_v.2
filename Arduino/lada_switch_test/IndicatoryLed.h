#ifndef INDICATORS_H
#define INDICATORS_H
#include "Arduino.h"

class IndicatoryLed{
  private:
    int led_row;
    int led_column;
    int bit_num;
    int onState;
    //we will update only if state has changed ... 
    void (*callback_function_on)(void);
    void (*callback_function_off)(void);
    bool callback_set;
    
  public:
    IndicatoryLed();
    IndicatoryLed(int bit_num, int row, int column);
    void begin(int bit_num, int row, int column);
    void switchOn();
    void switchOff();
    void setState(uint32_t coddedVal);
    void setCallbackFunction(void (*callback_function_on)(void),void (*callback_function_off)(void));
    static uint8_t maxRowPointer[8];
    static void backLight(bool stat, bool previosState);
    static bool stateChanged;
    
  };

#endif // INDICATORS_H
