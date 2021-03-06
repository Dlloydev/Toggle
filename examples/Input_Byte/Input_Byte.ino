/******************************************************************
  Input Byte Example:
  ===================
  This example demonstrates how you can debounce any byte variable
  in the sketch. The Arduino pin functions are not used.
  Open Serial Monitor to view the debounced byte status.
  *****************************************************************/

#include <Toggle.h>

const byte ledPin = LED_BUILTIN;

byte bit = 0; // choose bit(0-7)
byte Input;

Toggle myInput(&Input);

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  myInput.begin(Input);
  myInput.setInputMode(myInput.inputMode::input_byte); // debounce all bits
  myInput.setSamplePeriodUs(20); // 0-65535μs
}

void loop() {
  myInput.poll();
  if (myInput.onPress()) {
    Serial.print(F("b")); Serial.print(bit); Serial.println(F(": OFF⇒ON"));
  }
  if (myInput.onRelease()) {
    Serial.print(F("b")); Serial.print(bit); Serial.println(F(": ON⇒OFF"));
  }
  digitalWrite(ledPin, myInput.toggle());
}
