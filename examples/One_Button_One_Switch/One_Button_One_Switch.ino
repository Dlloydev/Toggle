/************************************************************************
  One Button and One Three Position Switch:
  =========================================
  This example that blinks an LED each time a button or 3-position switch
  has transitioned. All input pins will have their pullups enabled.
    • the built-in LED will blink once for each transition.
    • open Serial Monitor to view transition status.
  ***********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
const byte swPinA = 3;
const byte swPinB = 4;
const byte ledPin = LED_BUILTIN;

Toggle sw1(buttonPin);       // button
Toggle sw2(swPinA, swPinB);  // 3-position switch

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  sw1.poll();
  if (sw1.OFFtoON()) Serial.println(F("sw1: OFF⇒ON"));
  if (sw1.ONtoOFF()) Serial.println(F("sw1: ON⇒OFF"));
  digitalWrite(ledPin, sw1.getAllTransitions());
  delay(10);

  sw2.poll();
  if (sw2.UPtoMID()) Serial.println(F("sw2: UP⇒MID"));
  if (sw2.MIDtoDN()) Serial.println(F("sw2: MID⇒DN"));
  if (sw2.DNtoMID()) Serial.println(F("sw2: DN⇒MID"));
  if (sw2.MIDtoUP()) Serial.println(F("sw2: MID⇒UP"));
  digitalWrite(ledPin, sw2.getAllTransitions());
  delay(10);
}
