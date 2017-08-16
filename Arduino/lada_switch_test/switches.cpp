#include "switches.h"

int Switches::COUNTER = 0;
unsigned long  Switches::COMMON_STATE = 0;


Switches::Switches()
{

}

Switches::Switches(int pin, bool reversed, int codded_index)
{
    this->pin_index = pin;
    this->reversed= reversed;
    pinMode(pin_index, INPUT);
    if (codded_index == -1) {
        this->codded_index = COUNTER++;
    }

}

void Switches::read_current_status() {  
    int val = (reversed) ? LOW :HIGH;
    if (digitalRead(pin_index) == val) {
        bitSet(COMMON_STATE, codded_index);
        latest_status = true;
    } else {
        latest_status = false;
        bitClear(COMMON_STATE, codded_index);
    }
}

bool Switches::isActivated() {
    return latest_status;
}

void Switches::resetCommonState() {
    Switches::COMMON_STATE = 0;
}  

uint32_t Switches::getCommonState() {
    return Switches::COMMON_STATE;
}

void Switches::printState(){
  Serial.print((latest_status)?pin_index:0);//(int)latest_status);
  Serial.print(" ");
}
