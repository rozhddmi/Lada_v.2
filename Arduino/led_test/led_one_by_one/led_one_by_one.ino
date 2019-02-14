#include "ICMIMax7219.h"  
uint8_t row[8] ={0,0,0,0,0,0,0,0};
// The number of the Arduino pin that is connected to the data in pin. The data in pin is pin 1 on the MAX7219.
const uint8_t PIN_DATA = 26;//24;

// The number of the Arduino pin that is connected to the clock pin. The clock pin is pin 13 on the MAX7219.
const uint8_t PIN_CLOCK = 24;//26;

// The number of the Arduino pin that is connected to the load pin. The data in pin is pin 12 on the MAX7219.
const uint8_t PIN_LOAD = 27;

// The number of LED rows connected to the MAX7219.
const uint8_t ROW_COUNT = 8;
ICMIMax7219 max7219;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  max7219.begin(PIN_DATA, PIN_CLOCK, PIN_LOAD, ROW_COUNT);
  max7219.enable(true);
  max7219.setIntensity(16);
  max7219.clearDisplay();
  Serial.println(" Turned on: ");
 // delay(2000);
  //updateLed(row);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0;i <ROW_COUNT;i++){
    for (int k=0;k <ROW_COUNT;k++){
      set_led_state(true,i, k ,row);
      Serial.print("Row: ");
      Serial.print(i);
       Serial.print(" Column: ");
      Serial.print(k);
       Serial.print(" State: ");
      Serial.println("ON");
      updateLed(row);
      delay(1000);
    }  
  }
  
    for (int i=0;i <ROW_COUNT;i++){
    for (int k=0;k <ROW_COUNT;k++){
      set_led_state(false,i, k ,row);
      Serial.print("Row: ");
      Serial.print(i);
       Serial.print(" Column: ");
      Serial.print(k);
       Serial.print(" State: ");
      Serial.println("OFF");
      updateLed(row);
      delay(1000);
    }  
  }
}


void updateLed(uint8_t * row) {
  for (int i = 0; i < ROW_COUNT+1; i++) {
      max7219.setRow(i, row[i]);
  }
}

void set_led_state(bool state, int row, int column, uint8_t* rowValue) {
      if (state)
        bitSet(rowValue[row], column);
      else 
        bitClear(rowValue[row], column);
}
