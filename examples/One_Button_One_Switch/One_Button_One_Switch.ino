/******************************************************************
  One Button and One Three Position Switch:
  =========================================
  This example that blinks an LED each time the button
  has transitioned. All input pins will have their pullups enabled.
  Open the Serial Monitor to view transition status.
  *****************************************************************/

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
  sw1.begin(buttonPin);
  sw2.begin(swPinA, swPinB);
}

void loop() {
  sw1.poll();
  sw2.poll();

  if (sw1.onPress()) Serial.println(F("sw1: OFF⇒ON"));
  if (sw1.onRelease()) Serial.println(F("sw1: ON⇒OFF"));
  if (sw2.UPtoMID()) Serial.println(F("sw2: UP⇒MID"));
  if (sw2.MIDtoDN()) Serial.println(F("sw2: MID⇒DN"));
  if (sw2.DNtoMID()) Serial.println(F("sw2: DN⇒MID"));
  if (sw2.MIDtoUP()) Serial.println(F("sw2: MID⇒UP"));
  digitalWrite(ledPin, sw1.toggle());
}
