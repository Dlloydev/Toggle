/***********************************************
  USE A MOMENTARY PUSHBUTTON AS A TOGGLE SWITCH:
  The button is connected from input pin to GND.
  INPUT_PULLUP and debouncing are pre-configured.
  The built in LED changes state as configured.
  **********************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

Toggle button(buttonPin);

void setup() {
  button.begin(buttonPin);
  button.setToggleState(0);    // set initial state 0 or 1
  button.setToggleTrigger(0);  // set trigger onPress: 0, or onRelease: 1

  pinMode(ledPin, OUTPUT);
}

void loop() {
  button.poll();
  digitalWrite(ledPin, button.toggle());
}
