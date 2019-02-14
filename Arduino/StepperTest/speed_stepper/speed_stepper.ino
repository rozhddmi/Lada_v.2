
/* 
 Stepper Motor Control - one revolution
 
 This program drives a unipolar or bipolar stepper motor. 
 The motor is attached to digital pins 8 - 11 of the Arduino.
 
 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.  
 
  
 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe
 
 */

#include <Stepper.h>

const int stepsPerRevolution = 512;  // change this to fit the number of steps per revolution
                                     // for your motor
int m_speed = 20;
// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 28,27,29,31);            

//Stepper myStepper(stepsPerRevolution, 28,29,30,31);            
Stepper myStepper(stepsPerRevolution, 30,31,28,29);  

Stepper myStepper2(stepsPerRevolution, 33,32,34,35);   
void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(m_speed);
  myStepper2.setSpeed(m_speed);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
   Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
    myStepper2.step(stepsPerRevolution);

  delay(500);
  
   // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
    myStepper2.step(-stepsPerRevolution);

  delay(500); 
}

