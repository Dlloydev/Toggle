/************************************************************************
  Input logic Test Example:
  =========================
  A simple example that reads array bytes as the input signal.
  • open Serial Monitor to view/copy the results in csv format.
  • open Serial Plotter to view the noisy input and debounced outputs.
  • can be used to monitor, deglitch and debounce an input representing
  RPM pulses, flow meter pulses, noisy comparator output signal, etc.
  ***********************************************************************/

#include <Toggle.h>

const byte ledPin = LED_BUILTIN;
const byte dat[47] = {
  1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

byte logic;

Toggle sw1(&logic);

void setup() {
  pinMode(ledPin, OUTPUT);
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);

  sw1.setInputMode(sw1.inMode::input_logic);

  for (int i = 0; i < 47; i++) {
    logic = dat[i];
    sw1.poll();

    bool off = sw1.isOFF();
    bool on = sw1.isON();
    digitalWrite(ledPin, on);

    for (int i = 0; i < 10; i++) { // zoom horizontal
      plot("In", logic + 4, false);
      plot("isOFF", off + 2, false);
      plot("isON", on, true);
    }
    delay(50);
  }
}

void loop() {
}

void plot(String label, float value, bool last) {
  Serial.print(label); // can be empty
  if (label != "") Serial.print(":");
  Serial.print(value);
  if (last == false) Serial.print(", ");
  else Serial.println();
}
