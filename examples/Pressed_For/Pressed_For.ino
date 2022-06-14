/*****************************************************************
  PRESSED FOR EXAMPLE:
  While a button is pressed, returns true after elapsed ms
  Try on Wokwi UNO: https://wokwi.com/projects/334457647787934292
          ESP32-S2: https://wokwi.com/projects/334454109412262483
  ****************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte ledPin = LED_BUILTIN;
const unsigned int ms = 1000;

Toggle sw1(buttonPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  sw1.begin(buttonPin);
}

void loop() {
  sw1.poll();
  digitalWrite(ledPin, sw1.pressedFor(ms));
}
