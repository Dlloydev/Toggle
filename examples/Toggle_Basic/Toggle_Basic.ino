/************************************************************************
  Toggle Basic Example:
  =====================
  A simple example that toggles an LED each time a button is pressed
  or switch is closed. The input pin is pulled high and connected to GND.
  Therefore, when released (OFF),the signal is HIGH. When pressed (ON),
  the signal is LOW. The serial monitor indicates switch transitions.
  ***********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;
byte ledState = LOW;

Toggle sw1(buttonPin);
Toggle sw2(buttonPin + 1);

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  sw1.poll();
  sw2.poll();
  if (sw1.ONtoOFF()) Serial.println(F("sw1: ON⇒OFF"));
  if (sw1.OFFtoON()) {
    Serial.println(F("sw1: OFF⇒ON"));
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
