#include "indicators.h"

Indicators::Indicators(int bit_num, int row, int column,  int pinNum, uint8_t* maxRowPointer, int onState = HIGH) {
    this->row = row;
    this->column = column;
    this->bit_num = bit_num;
    this->maxRowPointer = maxRowPointer;
    this->onState = onState;

}
Indicators::~Indicators() {

}
void Indicators::switchOn() {
     bitSet(rowValue[row], column);
}
void Indicators::switchOff() {
     bitClear(rowValue[row], column);
}

void Indicators::setState(int coddedVal) {    // Set a state of the led

       if (digitalRead(i) == onState) {
           switchOn();
       } else {
           switchOff() ;
    }
}




