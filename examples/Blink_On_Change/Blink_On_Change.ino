/***********************************************************
  Toggle Blink On Change Example:
  ===============================
  A simple example that blinks an LED each time a button is
  pressed or released.
  **********************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

Toggle sw1(buttonPin);

void setup() {
  sw1.begin(buttonPin);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sw1.poll();
  digitalWrite(ledPin, sw1.blink(100, 2));
  sw1.onPress();
  sw1.onRelease();
}
