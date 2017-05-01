#ifndef SWITCHES_H
#define SWITCHES_H


class Switches
{
private:
    int codded_index;
    int pin_index;
    bool reversed;
    bool latest_status;
    static int counter;
public:
    Switches(int pin,bool reversed = false,int codded_index = -1);
    void read_current_status();
    void resetCommonState();
    uint32_t getCommonState();
    bool isActivated();
    static uint32_t common_state;

};

#endif // SWITCHES_H
