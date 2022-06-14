/****************************************************************
  TOGGLE BASIC EXAMPLE:
  This example blinks the built in LED as configured below.
  INPUT_PULLUP and debouncing are pre-configured.
  Try on Wokwi UNO: https://wokwi.com/projects/334443631893021266
          ESP32-S2: https://wokwi.com/projects/334453650605736531
  ***************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;

const unsigned int blinkMs = 100; // blink duration (ms)
const byte blinkMode = 1; // 0: on change, 1: on press (default), 2: on release

Toggle button(buttonPin);

void setup() {
  button.begin(buttonPin);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  button.poll();
  digitalWrite(ledPin, button.blink(blinkMs, blinkMode));
}
