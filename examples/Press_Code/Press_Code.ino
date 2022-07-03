/************************************************************************
  Press Code Example:
  ===================
  A simple example that demonstrates how fast-clicks, short presses and
  long presses can be automatically detected and combined into a byte code.
 
  ⦿ Up to 225 possible codes with one button. The returned code (byte) is easy to
    interpret when viewed in hex format. For example, `47` is 4 long, 7 short presses.
    F2 is double-click, F7 is 7 `F`ast clicks.

  ⦿ Fast-click mode is detected if the first several presses are less than 0.2 sec,
    then all presses are counted as fast, up to 15 max (code `FF`).

  ⦿ Detection of long presses occurs if the first press is greater than 0.2 sec,
    then all presses greater than 0.2 sec are counted as long
    and all presses less than 0.2 sec are counted as short presses.

  ⦿ Detect up to 15 short presses
  ⦿ Detect up to 14 long presses
  ⦿ Returns code after button is released for 0.5 sec
  ⦿ simplifies your code while adding maximum functionality to one button
  
  Try on Wokwi UNO: https://wokwi.com/projects/334284248581145170
          ESP32-S2: https://wokwi.com/projects/334320246564323924
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
