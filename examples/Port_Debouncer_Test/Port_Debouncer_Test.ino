/****************************************************************************
  Port Debouncer Test Example:
  ============================
  This example demonstrates how you can easily debounce a complete 8-bit port
  (8 signals) at a time in just one Toggle object. Several instances of Toggle
  can debounce 16-bit I/O expanders or other hardware, sensor or stored data.

  This is the expected serial print with leading 0's added:

  In: 00000000 Out: 00000000
  In: 10000001 Out: 00000000
  In: 10100011 Out: 00000000
  In: 00100111 Out: 00000001
  In: 00001110 Out: 00000011
  In: 00011110 Out: 00000111
  In: 00101111 Out: 00001111
  In: 01110110 Out: 00001111
  In: 11111100 Out: 00101111
  In: 11011000 Out: 01111110
  In: 01110000 Out: 01111100
  In: 01100011 Out: 01111000
  In: 11000000 Out: 01110000
  In: 10000100 Out: 01100000
  In: 00000000 Out: 01000000

  Looking at the columns (bit data) top to bottom, it can be seen that the
  debounced "Out" data lags by only 2 samples (rows). It also can be seen
  that the port debouncer can tolerate a very noisy signal with up to 2
  consecutive 1's or 0's that are anomalous or spurious in the In data.
  ***************************************************************************/

#include <Toggle.h>

const byte dat[15] = {
  0b00000000,
  0b10000001,
  0b10000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00101111,
  0b01110110,
  0b11111100,
  0b11011000,
  0b01110000,
  0b01100000,
  0b11000000,
  0b10000000,
  0b00000000
};

byte port;

Toggle sw1(&port);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);

  sw1.setInputMode(sw1.inMode::input_port);
  sw1.setSampleUs(100); // 100µs sample period

  for (int i = 0; i < 15; i++) {
    port = dat[i];
    sw1.poll();
    Serial.print(F("In: "));
    Serial.print(dat[i], BIN);
    Serial.print(F(" Out: "));
    Serial.println(sw1.debouncePort(), BIN);
  }
}

void loop() {
}
