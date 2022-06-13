/************************************************************************
  Press Code Example:
  ===================
  A simple example that demonstrates how fast-clicks, short presses and
  long presses can be automatically detected and combined into a byte code.
  In debug mode, the serial monitor shows timing results and code.
  ***********************************************************************/

#include <Toggle.h>

const byte buttonPin = 2;
byte code;

Toggle sw1(buttonPin);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  sw1.begin(buttonPin);
}

void loop() {
  sw1.poll();
  sw1.pressCode(1); // print: (1) on () off
}
