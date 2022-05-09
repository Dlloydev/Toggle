/************************************************************************
  Using Three Position Switches:
  ==============================
  A simple example that toggles an LED each time a 3-position switch has
  transitioned. Both input pins are pulled high and connected to GND. The
  switch is in the disconnected MID position if both inpits read high.
    • the built-in LED will blink once for each transition.
    • open Serial Monitor to view transition status.
  ***********************************************************************/

#include <Toggle.h>

const byte pinA = 2;
const byte pinB = 3;
const byte ledPin = LED_BUILTIN;

Toggle sw1(pinA, pinB);  // 3-position switch

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  sw1.poll();
  digitalWrite(ledPin, sw1.getAllTransitions());
  if (sw1.UPtoMID()) Serial.println(F("sw1: UP⇒MID"));
  if (sw1.MIDtoDN()) Serial.println(F("sw1: MID⇒DN"));
  if (sw1.DNtoMID()) Serial.println(F("sw1: DN⇒MID"));
  if (sw1.MIDtoUP()) Serial.println(F("sw1: MID⇒UP"));
}