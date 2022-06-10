/***********************************************************************
  Input Pulldown Example:
  =======================
  A simple example that toggles an LED each time a button is pressed
  or switch is closed.  On an ESP32, the input pin will have its
  pulldown enabled. The serial monitor indicates all switch transitions.
  **********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

Toggle sw1(buttonPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  sw1.begin(buttonPin);
  sw1.setInputMode(sw1.inputMode::input_pulldown);
  sw1.setToggleState(0);    // set initial state 0 or 1
  sw1.setToggleTrigger(0);  // set trigger onPress: 0, or onRelease: 1
}

void loop() {
  sw1.poll();
  digitalWrite(ledPin, sw1.toggle());
}
