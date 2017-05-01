#ifndef SWITCHES_H
#define SWITCHES_H
#include "Arduino.h"

class Switches
{
private:
    int codded_index;
    int pin_index;
    bool reversed;
    bool latest_status;
    static int COUNTER;
    static unsigned long  COMMON_STATE;
    
public:
    Switches();
    Switches(int pin,bool reversed = false,int codded_index = -1);
    void read_current_status();
    static void resetCommonState();  
    static uint32_t getCommonState();
    bool isActivated();
    void printState();
  };
  
#endif // SWITCHES_H

