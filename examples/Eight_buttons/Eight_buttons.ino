/************************************************************************
  Eight Buttons (or Switches) Example:
  ====================================
  An example that blinks an LED each time any one of eight buttons
  is pressed or released. All input pins will have their pullups enabled.
  The serial monitor indicates all switch transitions by pin number.
  ***********************************************************************/

#include <Toggle.h>

const byte ledPin = LED_BUILTIN;
const byte num = 8; // number of buttons
const byte pin[num] = {2, 3, 4, 5, 6, 7, 8, 9}; //button pins

Toggle sw[num] = {Toggle(pin[0]), Toggle(pin[1]), Toggle(pin[2]), Toggle(pin[3]), Toggle(pin[4]), Toggle(pin[5]), Toggle(pin[6]), Toggle(pin[7])};

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < num; i++) {
    sw[i].poll();
    if (sw[i].ONtoOFF()) {
      Serial.print("pin"); Serial.print(pin[i]); Serial.println(": ON⇒OFF");
    }
    if (sw[i].OFFtoON()) {
      Serial.print("pin"); Serial.print(pin[i]); Serial.println(": OFF⇒ON");
    }
    if (sw[i].getAllTransitions()) {
      digitalWrite(ledPin, HIGH);
      delay(10);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}
