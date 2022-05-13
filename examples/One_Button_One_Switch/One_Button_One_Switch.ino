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
  sw2.poll();

  if (sw1.OFFtoON()) {
    Serial.println(F("sw1: OFF⇒ON"));
    blink();
  }
  if (sw1.ONtoOFF()) {
    Serial.println(F("sw1: ON⇒OFF"));
    blink();
  }
  if (sw2.UPtoMID()) {
    Serial.println(F("sw2: UP⇒MID"));
    blink();
  }
  if (sw2.MIDtoDN()) {
    Serial.println(F("sw2: MID⇒DN"));
    blink();
  }
  if (sw2.DNtoMID()) {
    Serial.println(F("sw2: DN⇒MID"));
    blink();
  }
  if (sw2.MIDtoUP()) {
    Serial.println(F("sw2: MID⇒UP"));
    blink();
  }
}

void blink() {
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);
}
