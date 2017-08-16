#include <Stepper.h>   // edited Stepper library, supporting half-step mode
#include "ICMIMax7219.h"  // library for control of LED matrix using Maxim MAX7219
#include "msg_struct.h"
#include "IndicatoryLed.h"
#include "switches.h"


#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// communication variables and buffers
SDE_to_ARD * data_in  = new SDE_to_ARD;
SDE_to_PC * data_out = new SDE_to_PC;
int msg_size_to_pc  = sizeof(SDE_to_PC);
int msg_size_to_ard = sizeof(SDE_to_ARD);
char * buffIn = new char[msg_size_to_ard];
char * buffOut = new char[msg_size_to_pc];

// The number of the Arduino pin that is connected to the data in pin. The data in pin is pin 1 on the MAX7219.
const uint8_t PIN_DATA = 24;

// The number of the Arduino pin that is connected to the clock pin. The clock pin is pin 13 on the MAX7219.
const uint8_t PIN_CLOCK = 26;

// The number of the Arduino pin that is connected to the load pin. The data in pin is pin 12 on the MAX7219.
const uint8_t PIN_LOAD = 27;

// The number of LED rows connected to the MAX7219.
const uint8_t ROW_COUNT = 8;




//Initialize a variable for controlling the MAX7219.
ICMIMax7219 max7219;
//-------------------
const int Rele = 50;                         // Pin to which indicator relay is connected
const int hazard_sw = 46;                    // Pin responsible for lighting the diode in hazard switch button


int maxVal = 450, minVal = 80;              // Extreme values on the output of AD converter if gas pedal is in extreme positions
int procent;                                // Variable used for recalculating of pressing the gas to percent measure
int acceleratorPin = A11;                   // Pin to which slider of the potenciometer in gas pedal is connected
int acceleratorValue = 0;                   // Variable for storing current value of gas pedal

// Definition of the two steppers (to which pins are they connected and if the 6th parameter is 1, the motor operates in half-step mode)

const int stepsPerRevolution = 512;         // number of steps between extreme positions of stepper motors (for half-step mode is twice as much)
const int steperSpeed = 20;   
//Stepper myStepper(1020, 22, 23, 25, 24, 1);
//Stepper myStepper2(1020, 26, 27, 29, 28, 1);

Stepper myStepper(stepsPerRevolution, 30,31,28,29);  
Stepper myStepper2(stepsPerRevolution, 33,32,34,35);  


const int max_rpm = 9000;//10000;                   // Max number of RPM that can be displayed on dash
const int max_speed = 200;//240;                 // Max speed that can be displayed on dash
int steps_rmp;                                  // No. of steps the motor has to do to get to desired value of RPM
int steps_speed;                                // No. of steps the motor has to do to get to desired value of RPM
int current_speed = 0;                               // Variable storing current position of stepper (storing the number of steps from the beginning)
int current_rmp = 0;                               // Variable storing current position of stepper (storing the number of steps from the beginning)
const float delimiter_rmp = (float)stepsPerRevolution / max_rpm;
const float delimiter_speed = (float)stepsPerRevolution / max_speed;
 
 // CALLIBRATION parameters for speed and RPM
unsigned long  run_time = 0;
unsigned long last_zero_calibration_speed = 0;
unsigned long last_zero_calibration_rmp = 0;
int callibration_periud = 2000; // 



Switches  switches[17] = { 
  Switches(2),//WIPERS_1  ostricovac
  Switches(3), //WIPERS_2  speed 3
  Switches(4), //WIPERS_2  speed 2
  Switches(5), //WIPERS_3 speed 1
  Switches(6), //WIPERS_4  zadni stirac
  Switches(7), //WIPERS_4  6zadni ostrikovac
  
  Switches(9), //LEFT_BL 7
  Switches(8), //RIGHT_BL  4
  
  Switches(10,true), // FOG LIGHT
  //Switches(-1,true), // 

  Switches(12,true), // BEAM LIGHT
  Switches(11,true), // PARK_LIGHT  
  Switches(13,true), // LOW LIGHT
   Switches(18,true), 
  Switches(19,true), 
  //Switches(-1,true), // 
  Switches(21,true), // IGNITION
  Switches(20,true), // STARTER
  //Switches(-1,true), //
};



void setup() {
  max7219.begin(PIN_DATA, PIN_CLOCK, PIN_LOAD, ROW_COUNT);        // Setup of MAX7219

  // Definition of output pins
  pinMode(Rele, OUTPUT);
  pinMode(hazard_sw, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(1000);

}

void loop() {
  run_time = millis();
  delay(1);

 
 // Reading all needed inputs
 for (int i =0; i < NELEMS(switches); i++) {
    switches[i].read_current_status();
    Serial.print(switches[i].isActivated()?"1 ":"0 ");
    //switches[i].printState();
 }
 Serial.print(Switches::getCommonState());
 Serial.println(" - "); 
 //Serial.println(Switches::getCommonState());

}





