/*********************************************************************
  ESP32-S2 INPUT_PULLDOWN EXAMPLE:
  This example blinks the built in LED as configured below.
  Try on Wokwi ESP32-S2: https://wokwi.com/projects/334554744838160980
  ********************************************************************/

#include <Toggle.h>

const byte buttonPin = 4;
const byte ledPin = 16;

const unsigned int blinkMs = 100; // blink duration (ms)
const byte blinkMode = 1; // 0: on change, 1: on press (default), 2: on release

Toggle button(buttonPin);

void setup() {
  button.begin(buttonPin);
  button.setInputInvert(1);
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  button.poll();
  digitalWrite(ledPin, button.blink(blinkMs, blinkMode));
  delay(1);
}
