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
int acceleratorValue = 0;                   // Variable for storing current value of gas pedal

// Definition of the two steppers (to which pins are they connected and if the 6th parameter is 1, the motor operates in half-step mode)

const int stepsPerRevolution = 512;         // number of steps between extreme positions of stepper motors (for half-step mode is twice as much)
const int steperSpeed = 20;   
//Stepper myStepper(1020, 22, 23, 25, 24, 1);
//Stepper myStepper2(1020, 26, 27, 29, 28, 1);

Stepper myStepper2(stepsPerRevolution, 30,31,28,29);  
Stepper myStepper(stepsPerRevolution, 33,32,34,35);  


const int max_rpm = 10000;                   // Max number of RPM that can be displayed on dash
const int max_speed = 240;                 // Max speed that can be displayed on dash
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

IndicatoryLed  indicatory[22] = {
   IndicatoryLed(0,6,6), // imobilizer
   IndicatoryLed(1,1,3), // ABS
   IndicatoryLed(2,3,3), // check engine
   IndicatoryLed(3,0,4), // airbag
   IndicatoryLed(4,0,3), // OIL
   IndicatoryLed(5,5,3), // batery
   IndicatoryLed(6,6,3), // Fog light
   IndicatoryLed(7,6,5), // Podkovacki
   IndicatoryLed(8,7,5), // Dalkovy
   IndicatoryLed(9,5,6), // left blnk
   IndicatoryLed(10,7,6), // right blnk
   
   IndicatoryLed(11,4,3), // water temp
   IndicatoryLed(12,4,4), // tyre presure
   IndicatoryLed(13,6,4), // doors
   IndicatoryLed(14,2,4), // seetbelt
   IndicatoryLed(15,2,3), // Hand brake
   IndicatoryLed(17,7,3), // fog light back
   IndicatoryLed(18,1,4), // some gear
   IndicatoryLed(19,3,4), // steering wheel 
   IndicatoryLed(20,5,4), // Refuel
   IndicatoryLed(21,7,4), // attention
   

   
};

IndicatoryLed * left = &indicatory[9];
IndicatoryLed * right = &indicatory[10];

Switches  switches[17] = { 
  Switches(2),//WIPERS_1  ostricovac
  Switches(3), //WIPERS_2  speed 3
  Switches(4), //WIPERS_2  speed 2
  Switches(5), //WIPERS_3 speed 1
  Switches(6), //WIPERS_4  zadni stirac
  Switches(7), //WIPERS_4  6zadni ostrikovac
  
  Switches(8), //LEFT_BL 7
  Switches(9), //RIGHT_BL  4
  
  Switches(10,true), // FOG LIGHT
  Switches(-1,true), // 

  Switches(12,true), // BEAM LIGHT
  Switches(11,true), // PARK_LIGHT  
  Switches(13,true), // LOW LIGHT
  Switches(-1,true), // 
  Switches(21,true), // IGNITION
  Switches(20,true), // STARTER
  Switches(-1,true), // 

};
Switches * light = &switches[11];
bool backLight_pevState;


void setup() {
  max7219.begin(PIN_DATA, PIN_CLOCK, PIN_LOAD, ROW_COUNT);        // Setup of MAX7219

  // Definition of output pins
  pinMode(Rele, OUTPUT);
  pinMode(hazard_sw, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(1000);
  // Setting of steppers speed
  myStepper.setSpeed(steperSpeed);//42
  myStepper2.setSpeed(steperSpeed);//42


  max7219.enable(true);
  max7219.setIntensity(16);
  max7219.clearDisplay();
  // resseting current input 
  data_in->indicators = 0;
  data_in->speed = 0;
  data_in->rmp= 0;
  data_in->engine_state= 0;
  backLight_pevState = !light->isActivated();
  
  // handbrack cheat we set one analog input to low 
  // pull up othret as an input and read a value
  
  left->setCallbackFunction(&relleyOn,&relleyOff);
  right->setCallbackFunction(&relleyOn,&relleyOff);
}

void loop() {
  run_time = millis();
  delay(1);

  // computation of accelerator press degree
  acceleratorValue = 2;
  procent = (float)(acceleratorValue - minVal) / (maxVal - minVal) * 100;
  // rotating stepper
  updateSteper (myStepper, steps_rmp, current_rmp,last_zero_calibration_rmp);
  updateSteper (myStepper2, steps_speed, current_speed,last_zero_calibration_speed);
 
 // Reading all needed inputs
 for (int i =0; i < NELEMS(switches); i++) {
    switches[i].read_current_status();
    //switches[i].printState();
 }
 //Serial.print(" - ");
 //Serial.println(Switches::getCommonState());

 // switching back light if needed
 IndicatoryLed::backLight(light->isActivated(),backLight_pevState);
 backLight_pevState = light->isActivated();
 // Sertting led status
  //Serial.println(sizeof(SDE_to_PC));
  for (int i =0; i < NELEMS(indicatory); i++) {
      indicatory[i].setState(data_in->indicators);
   }
  
  if (IndicatoryLed::stateChanged){
    updateLed(IndicatoryLed::maxRowPointer);
  }
 
  /**  steps_rmp = (ceil)(data_in->rmp * delimiter_rmp);
      steps_speed = (ceil)(data_in->speed * delimiter_speed);     
      data_out->displayed_speed = 2;
      data_out->displayed_rmp =4;
      data_out->buttonState = 8;
      data_out->acceleratorValue = 16; 
      data_out->brakeValue = 32; 
      data_out->clutchValue = 64; 
  Serial.print(data_out->buttonState);Serial.print(" ");
      Serial.print(data_out->displayed_speed);Serial.print(" ");
      Serial.print(data_out->displayed_rmp);Serial.print(" ");
      Serial.print(data_out->acceleratorValue);Serial.print(" ");
      Serial.print(data_out->brakeValue);Serial.print(" ");
      Serial.print(data_out->clutchValue);Serial.print(" ");
      Serial.println(sizeof(SDE_to_PC));
      **/
  if (true && Serial.available() >= msg_size_to_ard ) {
    if (readSerial(data_in)) {
      steps_rmp = (ceil)(data_in->rmp * delimiter_rmp);
      steps_speed = (ceil)(data_in->speed * delimiter_speed);     
       data_out->displayed_speed = 2;
      data_out->displayed_rmp =4;
      data_out->buttonState = 8;
      data_out->acceleratorValue = 16; 
      data_out->brakeValue = 32; 
      data_out->clutchValue = 64; 
      /**Serial.print(data_out->buttonState);Serial.print(" ");
      Serial.print(data_out->displayed_speed);Serial.print(" ");
      Serial.print(data_out->displayed_rmp);Serial.print(" ");
      Serial.print(data_out->acceleratorValue);Serial.print(" ");
      Serial.print(data_out->brakeValue);Serial.print(" ");
      Serial.print(data_out->clutchValue);Serial.print(" ");
      Serial.println("|");**/
      writeSerial(data_out);
    }
  }

}



void updateSteper(Stepper & stepper, int steps, int & current,unsigned long & last_updated) {
  int dif = abs(steps - current);
  int incr;
  if (dif >10){
    incr=1;
  } else {
    incr = 1;
  }
  
  if (steps > current) {
    stepper.step(incr);
    current+=incr;
  }  else if (steps < current)  {
    stepper.step(-incr);
    current-=incr;
  }
  current = MAX(0,current);
  // self adjustment ifcurrent is zero for some time (udjustable) we
  // make one step down 
  if (current == 0 && (run_time - last_updated) > callibration_periud) {
    stepper.step(-incr);
    last_updated = run_time;
  }
}


bool readSerial(SDE_to_ARD* dataRecieved) {
  if (Serial.readBytes(buffIn, msg_size_to_ard) ) {
    memcpy(data_in, buffIn, msg_size_to_ard);
    return true;
  }
  return false;
}


bool writeSerial(SDE_to_PC* dataToSend) {
  memcpy(buffOut, data_out, msg_size_to_pc);
  Serial.write(buffOut, msg_size_to_pc);
}


bool initCommunication() {
  SDE_to_PC * data_out_init = new SDE_to_PC;
  memcpy(buffOut, data_out_init, msg_size_to_pc);
  Serial.write(buffOut, msg_size_to_pc);
}


void makeXsteps(Stepper & stepper, int steps) {
  int steps_done= 0;
  int sign = (int) steps/ abs(steps);
  while (steps_done < abs(steps)) {
    stepper.step(sign);
    steps_done++;
  }
}

void updateLed(uint8_t * row) {
  for (int i = 0; i < ROW_COUNT; i++) {
      max7219.setRow(i, row[i]);
  }
}

void set_led_state(bool state, int row, int column, uint8_t* rowValue) {
      if (state)
        bitSet(rowValue[row], column);
      else 
        bitClear(rowValue[row], column);
}

void relleyOn(void) {
  digitalWrite(Rele,HIGH);
   digitalWrite(hazard_sw,HIGH);
}

void relleyOff(void) {
   digitalWrite(Rele,LOW);
   digitalWrite(hazard_sw,LOW);
}
