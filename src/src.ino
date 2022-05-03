#include <Toggle.h>

Toggle sw1(7, 8);

void setup() {
  Serial.begin(115200);
}

void loop() {
  sw1.poll();

  if      (sw1.upToMid()) Serial.println(F("Up⇒Mid"));
  else if (sw1.dnToMid()) Serial.println(F("Dn⇒Mid"));
  else if (sw1.midToUp()) Serial.println(F("Mid⇒Up"));
  else if (sw1.midToDn()) Serial.println(F("Mid⇒Dn"));
}
