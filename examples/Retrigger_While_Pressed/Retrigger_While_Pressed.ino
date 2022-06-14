/**************************************************************************
  Retrigger While Pressed Example:
  ================================
  An example that shows how to retrigger an event while a button is pressed.
  The retrigger period (ms) determines the rate. Each expired time period is
  counted and displayed on the serial monitor. The built in LED toggles on
  each trigger event.
  Try on Wokwi UNO: https://wokwi.com/projects/334458963671122515
          ESP32-S2: https://wokwi.com/projects/334459280465855059
  *************************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const word retriggerMs = 1000;
const byte ledPin = LED_BUILTIN;
bool ledState = false;
byte trigCount = 0;

Toggle sw1(buttonPin);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  sw1.begin(buttonPin);
}

void loop() {
  sw1.poll();
  if (sw1.retrigger(retriggerMs)) {
    trigCount++;
    Serial.println(trigCount);
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  if (sw1.onRelease()) {
    Serial.println(F("Released"));
    trigCount = 0;
  }
}
