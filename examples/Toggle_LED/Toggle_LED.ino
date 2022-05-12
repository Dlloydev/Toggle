/************************************************************************
  Toggle LED Example:
  ===================
  A simple example that toggles an LED each time a button is pressed
  or switch is closed. The input pin will have an external pulldown
  resistor connected. The serial monitor indicates switch transitions.
  ***********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;
byte ledState = LOW;

Toggle sw1(buttonPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  sw1.setInputMode(sw1.inMode::input);
  sw1.setInvertMode(true);
}

void loop() {
  sw1.poll();
  if (sw1.OFFtoON()) {
    Serial.println(F("sw1: OFF⇒ON"));
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  if (sw1.ONtoOFF()) Serial.println(F("sw1: ON⇒OFF"));
}
