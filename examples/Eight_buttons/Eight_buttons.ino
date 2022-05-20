/***********************************************************
  Eight Buttons (or Switches) Example:
  ====================================
  An example that checks the status of eight buttons.
  All input pins will have their pullups enabled.
  The LED toggles for each on press transition.
  **********************************************************/

#include <Toggle.h>

const byte num = 8; // number of buttons
const byte pin[num] = {2, 3, 4, 5, 6, 7, 8, 9}; //button pins
const byte ledPin = 13;
byte ledState = LOW;

Toggle *sw = new Toggle[num];

void setup() {
  for (int i = 0; i < num; ++i) {
    sw[i].begin(pin[i]);
    sw[i] = pin[i];
  }
}

void loop() {

  for (int i = 0; i < num; i++)  {
    sw[i].poll();
    if (sw[i].onPress()) ledState = !ledState;
  }
  digitalWrite(ledPin, ledState);
}
