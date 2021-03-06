/****************************************************
  EIGHT BUTTONS EXAMPLE:
  An example that checks the status of eight buttons.
  All input pins will have their pullups enabled.
  The LED blinks for each on press transition.
  ***************************************************/

#include <Toggle.h>

const byte num = 8; // number of buttons
const byte pin[num] = {2, 3, 4, 5, 6, 7, 8, 9}; //button pins
const byte ledPin = LED_BUILTIN;
const unsigned int blinkMs = 100; // led blink duration (ms)

Toggle *sw = new Toggle[num];

void setup() {
  for (int i = 0; i < num; ++i) {
    sw[i].begin(pin[i]);
    sw[i] = pin[i];
  }
}

void loop() {
  byte ledState = num;
  for (int i = 0; i < num; i++)  {
    sw[i].poll();
    if (sw[i].blink(blinkMs)) {
      ledState++;
    } else {
      ledState--;
    }
  }
  digitalWrite(ledPin, ledState);
}
