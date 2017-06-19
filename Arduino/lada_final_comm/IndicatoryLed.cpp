#include "IndicatoryLed.h"

uint8_t IndicatoryLed::maxRowPointer[8] ={0,0,0,0,0,0,0,0};
bool IndicatoryLed::stateChanged = true;


IndicatoryLed::IndicatoryLed() {
   this->led_row = -1;
    this->led_column = -1;
    this->bit_num = -1;  
    this->callback_set = false;
}

IndicatoryLed::IndicatoryLed(int bit_num, int row, int column) {
   this->led_row = row;
    this->led_column = column;    
    this->bit_num = bit_num;
    this->callback_set = false;
}

void IndicatoryLed::begin(int bit_num, int row, int column) {
    this->led_row = row;
    this->led_column = column;
    this->bit_num = bit_num;
    this->callback_set = false;
}


void IndicatoryLed::switchOn() {
  if (!bitRead(maxRowPointer[led_row], led_column)) {   
    bitSet(maxRowPointer[led_row], led_column);    
    IndicatoryLed::stateChanged = true;
    if (this->callback_set) {
      callback_function_on();
    }
  }
}

void IndicatoryLed::switchOff() {
  if (bitRead(maxRowPointer[led_row], led_column)) {   
    bitClear(maxRowPointer[led_row], led_column); 
    IndicatoryLed::stateChanged = true;
    if (this->callback_set) {
      callback_function_off();
    }
  }  
}

void IndicatoryLed::setState(uint32_t coddedVal) {    // Set a state of the led 
  
       if (bitRead(coddedVal,bit_num)) {
           switchOn();
       } else {
           switchOff() ;
    }
}
        
 void IndicatoryLed::backLight(bool stat, bool previosState) 
{
  if (stat != previosState) {
    IndicatoryLed::stateChanged = true;
    if (!stat ) {
          bitClear(maxRowPointer[0], 5);
          bitClear(maxRowPointer[0], 6);
          bitClear(maxRowPointer[1], 5);
          bitClear(maxRowPointer[1], 6);
          bitClear(maxRowPointer[2], 2);
          bitClear(maxRowPointer[2], 5);
          bitClear(maxRowPointer[2], 6);
          bitClear(maxRowPointer[3], 5);
          bitClear(maxRowPointer[3], 6);
          bitClear(maxRowPointer[4], 4);
          bitClear(maxRowPointer[4], 6);
          bitClear(maxRowPointer[5], 5);
        } else   {
          bitSet(maxRowPointer[0], 5);
          bitSet(maxRowPointer[0], 6);
          bitSet(maxRowPointer[1], 5);
          bitSet(maxRowPointer[1], 6);
          bitSet(maxRowPointer[2], 2);
          bitSet(maxRowPointer[2], 5);
          bitSet(maxRowPointer[2], 6);
          bitSet(maxRowPointer[3], 5);
          bitSet(maxRowPointer[3], 6);
          bitSet(maxRowPointer[4], 4);
          bitSet(maxRowPointer[4], 6);
          bitSet(maxRowPointer[5], 5);
       }
  }
}

void IndicatoryLed::setCallbackFunction(void (*callback_function_on)(void),void (*callback_function_off)(void)){
  this->callback_function_on = callback_function_on;
  this->callback_function_off = callback_function_off;
  this->callback_set = true;
}

