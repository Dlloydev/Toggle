#include <Toggle.h>

Toggle sw1(6,7); //SP3T
Toggle sw2(8);   //SPDT

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  sw1.poll();
  sw2.poll();

  if (sw1.OFFtoON) Serial.println(F("sw1: OFF⇒ON"));
  if (sw1.ONtoOFF) Serial.println(F("sw1: ON⇒OFF"));

  if (sw2.OFFtoON) Serial.println(F("sw2: OFF⇒ON"));
  if (sw2.ONtoOFF) Serial.println(F("sw2: ON⇒OFF"));

  if (sw1.UPtoMID) Serial.println(F("sw1: UP⇒MID"));
  if (sw1.MIDtoDN) Serial.println(F("sw1: MID⇒DN"));
  if (sw1.DNtoMID) Serial.println(F("sw1: DN⇒MID"));
  if (sw1.MIDtoUP) Serial.println(F("sw1: MID⇒UP"));

  if (sw2.UPtoMID) Serial.println(F("sw1: UP⇒MID"));
  if (sw2.MIDtoDN) Serial.println(F("sw1: MID⇒DN"));
  if (sw2.DNtoMID) Serial.println(F("sw1: DN⇒MID"));
  if (sw2.MIDtoUP) Serial.println(F("sw1: MID⇒UP"));
}
