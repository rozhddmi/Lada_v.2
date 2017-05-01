#include "switches.h"

int Switches::counter = 0;
uint32_t Switches::common_state = 0;

Switches::switches(int pin, bool reversed, int codded_index)
{
    this->pin_index = pin;
    this->reversed= reversed;
    if (codded_index == -1) {
        this->codded_index = counter++;
    }

}

void read_current_status() {
    int val = (reversed) ? LOW :HIGH;
    if (digitalRead(pin_index) == val) {
        bitSet(common_state, codded_index);
        latest_status = true;
    } else {
        latest_status = false;
    }
}

bool isActivated() {
    return latest_status;
}

void resetCommonState() {
    Switches::common_state = 0;
}

uint32_t getCommonState() {
    return Switches::common_state;
}
