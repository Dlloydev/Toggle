/**************************************************************************
  Input Bit Test Example:
  =======================
  This example demonstrates how you can debounce any bit of a byte variable
  in the sketch. The Arduino pin functions are not used.
  • Open Serial Plotter to view the noisy input and debounced outputs.
  • Open Serial Monitor to view/copy the results in CSV format.
  • Can be used to monitor, deglitch and debounce an input representing
  RPM pulses, flow meter pulses, noisy comparator output signal, etc.
  *************************************************************************/

#include <Toggle.h>

const byte dat[47] = {
  1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

byte Input;
byte bit = 0;

Toggle myInput(&Input);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
  myInput.begin(Input);
  myInput.setInputMode(myInput.inMode::input_port);
  myInput.setAlgorithm(2); // ignore(2), (1) or (0) glitches for robust, normal and quick response
  myInput.setSamplePeriodUs(20); // 0-65535μs

  for (int i = 0; i < 47; i++) {
    Input = dat[i] << bit;
    myInput.poll(bit);

    for (int i = 0; i < 10; i++) { // zoom horizontal
      plot("In", Input + 8, false);
      plot("isOFF", myInput.isReleased(bit) + 6, false);
      plot("OFF2ON", myInput.onPress() + 4, false);
      plot("ON2OFF", myInput.onRelease() + 2, false);
      plot("isON", myInput.isPressed(bit), true);
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
