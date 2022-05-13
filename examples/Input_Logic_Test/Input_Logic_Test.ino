/************************************************************************
  Input logic Test Example:
  =========================
  A simple example that reads array bytes as the input signal.
  • open Serial Plotter to view the noisy input and debounced outputs.
  • open Serial Monitor to view/copy the results in csv format.
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
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);

  sw1.setInputMode(sw1.inMode::input_port);
  sw1.setSampleUs(100); // 100µs sample period

  for (int i = 0; i < 47; i++) {
    logic = dat[i];
    sw1.poll();

    for (int i = 0; i < 10; i++) { // zoom horizontal
      plot("In", logic + 8, false);
      plot("isOFF", sw1.isOFF() + 6, false);
      plot("OFF2ON", sw1.OFFtoON() + 4, false);
      plot("ON2OFF", sw1.ONtoOFF() + 2, false);
      plot("isON", sw1.isON(), true);
    }
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
