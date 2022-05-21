/************************************************************************
  Toggle Basic Example:
  =====================
  A simple example that toggles an LED each time a button is pressed
  or switch is closed.  The input pin will have its pullup enabled.
  The serial monitor indicates all switch transitions.
  ***********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

Toggle sw1(buttonPin);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  sw1.begin(buttonPin);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sw1.poll();
  if (sw1.onPress()) Serial.println(F("sw1: OFF⇒ON"));
  if (sw1.onRelease()) Serial.println(F("sw1: ON⇒OFF"));
  digitalWrite(ledPin, sw1.toggle());
}
