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

const int stepsPerRevolution = 280;         // number of steps between extreme positions of stepper motors (for half-step mode is twice as much)
const int steperSpeed = 5;   
//Stepper myStepper(1020, 22, 23, 25, 24, 1);
//Stepper myStepper2(1020, 26, 27, 29, 28, 1);

Stepper myStepper(1020, 29, 31, 28, 30, 0);
Stepper myStepper2(1020, 33, 35, 32, 34, 0);


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

IndicatoryLed  indicatory[21] = {
   IndicatoryLed(0,2,6), // imobilizer
   IndicatoryLed(1,0,5), // ABS
   IndicatoryLed(2,5,3), // check engine
   IndicatoryLed(3,4,5), // airbag
   IndicatoryLed(4,5,5), // OIL
   IndicatoryLed(5,7,5), // Unknown
   IndicatoryLed(6,5,4), // Fog light
   IndicatoryLed(7,4,6), // Podkovacki
   IndicatoryLed(8,6,5), // Dalkovy
   IndicatoryLed(9,0,6), // left blnk
   IndicatoryLed(10,1,4), // right blnk
   
   IndicatoryLed(11,1,3), // water temp
   IndicatoryLed(12,1,5), // tyre presure
   IndicatoryLed(13,2,4), // doors
   IndicatoryLed(14,2,5), // seetbelt
   IndicatoryLed(15,3,5), // Hand brake
   IndicatoryLed(16,5,2), // airbag
   IndicatoryLed(17,4,3), // fog light back
   IndicatoryLed(18,5,6), // some gear
   IndicatoryLed(19,6,3), // steering wheel 
   IndicatoryLed(20,6,4), // Refuel
   
};

IndicatoryLed * left = &indicatory[9];
IndicatoryLed * right = &indicatory[10];

Switches  switches[17] = { 
  Switches(2),//WIPERS_1  2 
  Switches(3), //WIPERS_2  3
  Switches(5), //WIPERS_3 5
  Switches(6), //WIPERS_4  6
  Switches(12), //WIPERS_5  12
  Switches(13), //WIPERS_6  13
  
  Switches(7), //LEFT_BL 7
  Switches(4), //RIGHT_BL  4
  Switches(38,true), //FOG_LIGHT  38
  Switches(39,true), // ????
  Switches(40,true), //BEEM_LIGHT  40
  Switches(41,true), // PARK_LIGHT  41
  Switches(42,true), //LOW_LIGHT  42
  
  Switches(38,true), //GNITION_2 38
  Switches(36,true), //IGNITION_1 36
  Switches(39,true), // IGNITION_3  39
  Switches(A15), // HANDBRAKE ANALOG A15
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

  // Rotation of steppers to max position and back to zero position
  makeXsteps(myStepper,stepsPerRevolution);
  makeXsteps(myStepper,-stepsPerRevolution-10);
  makeXsteps(myStepper2,stepsPerRevolution); // make sure we are at zero
  makeXsteps(myStepper2,-stepsPerRevolution-10); // make sure we are at zero

  max7219.enable(true);
  max7219.setIntensity(16);
  // resseting current input 
  data_in->indicators = 0;
  data_in->speed = 0;
  data_in->rmp= 0;
  data_in->engine_state= 0;
  backLight_pevState = !light->isActivated();
  
  // handbrack cheat we set one analog input to low 
  // pull up othret as an input and read a value
  pinMode(A14, OUTPUT);
  digitalWrite(A14, LOW);
  digitalWrite(A15, HIGH);
  
  left->setCallbackFunction(&relleyOn,&relleyOff);
  right->setCallbackFunction(&relleyOn,&relleyOff);
}

void loop() {
  run_time = millis();
  delay(1);

  // computation of accelerator press degree
  acceleratorValue = analogRead(acceleratorPin);
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

  if (true && Serial.available() >= msg_size_to_ard ) {
    if (readSerial(data_in)) {
      steps_rmp = (ceil)(data_in->rmp * delimiter_rmp);
      steps_speed = (ceil)(data_in->speed * delimiter_speed);     
      data_out->displayed_speed = (int)current_speed / delimiter_speed;
      data_out->displayed_rmp =(int) current_rmp / delimiter_rmp;
      data_out->buttonState = Switches::getCommonState();
      data_out->acceleratorValue = procent; 
      data_out->brakeValue = 0; 
      data_out->clutchValue = 0; 
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
