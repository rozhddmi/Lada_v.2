#ifndef MSG_STRUCT
#define MSG_STRUCT

#include <stdint.h>


  struct SDE_to_PC {
      unsigned long buttonState;
      unsigned long displayed_speed;
      unsigned long displayed_rmp;  
      unsigned long acceleratorValue;
      unsigned long brakeValue;
      unsigned long clutchValue;     
};

struct SDE_to_ARD {
      unsigned long speed;
      unsigned long rmp;
      unsigned long indicators;
      unsigned long engine_state;
};
#endif

