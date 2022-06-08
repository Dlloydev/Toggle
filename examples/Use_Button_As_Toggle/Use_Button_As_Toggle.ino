/*******************************************************************************
  TURN A MOMENTARY PUSHBUTTON INTO A TOGGLE SWITCH:
  The button is connected from input pin to GND.
  INPUT_PULLUP and debouncing are pre-configured.
  The built in LED changes state each time the button is pressed.
  For the LED to change state on release, un-comment button.setInvertMode(1);
  For LED startup in opposite state, use digitalWrite(ledPin, button.toggle(1));
  ******************************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

Toggle button(buttonPin);

void setup() {
  button.begin(buttonPin);
  //button.setInvertMode(1);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  button.poll();
  digitalWrite(ledPin, button.toggle());
}
