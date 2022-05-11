/****************************************************
   Toggle Library for Arduino - Version 2.3.0
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ****************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle(uint8_t pinA) : _pinA(pinA) { }
Toggle::Toggle(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB) { }
Toggle::Toggle(uint8_t *logic) : _logic(logic) { }

void Toggle::poll() {
  if (firstRun) {
    firstRun = false;
    lastUs = micros();
    if (_pinA) {
      states |= 0b00010000; // lastOFF = 1
      if (_inputMode == inMode::input_pullup) pinMode(_pinA, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_pinA, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_pinA, INPUT_PULLDOWN);
#endif
    }
    if (_pinB) {
      if (_inputMode == inMode::input_pullup) pinMode(_pinB, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_pinB, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_pinB, INPUT_PULLDOWN);
#endif
    }
    if (_inputMode == inMode::input_port) {
      regA = 0;
      regB = 0;
      lastRegA = 0;
      lastRegB = 0;
    }
  }
  if (_inputMode != inMode::input_port) {
    lastRegA = regA;
    lastRegB = regB;
  }
  if (micros() - lastUs > _sampleUs) {
    lastUs += _sampleUs;
    if (_inputMode == inMode::input || _inputMode == inMode::input_pullup || _inputMode == inMode::input_pulldown) {
      if (_pinA) {
        if (states & 0b10000000) regA = regA << 1 | !digitalRead(_pinA); //invert
        else regA = regA << 1 | digitalRead(_pinA);
      }
      if (_pinB) {
        if (states & 0b10000000) regB = regB << 1 | !digitalRead(_pinB); //invert
        else regB = regB << 1 | digitalRead(_pinB);
      }
    }
    else if (_inputMode == inMode::input_logic) {
      uint8_t tmp = *_logic;
      if (states & 0b10000000) tmp = !tmp; //invert
      regA = regA << 1 | tmp;
    }
  }
  if (_pinA && _pinB) setStatesTwo();
  else if (_pinA || _inputMode == inMode::input_logic) setStatesOne();
}

void Toggle::setStatesOne() {
  // when using one input b4:lastOFF, b3:ONtoOFF, b2:OFFtoON, b1:isON, b0:isOFF;
  if ((lastRegA & 7) == 7) {
    states |= 0b00000001; // isOFF = 1
    states &= 0b11111101; // isON = 0
  }
  if ((regA & 3) == 0) {
    states |= 0b00000010; // isON = 1
    states &= 0b11111110; // isOFF = 0
  }
  states &= 0b11111011; // OFFtoON = 0
  if ((states & 0b00010000) && (states & 0b00000001) == 0) states |= 0b00000100; // OFFtoON = 1

  states &= 0b11110111; // ONtoOFF = 0
  if ((states & 0b00010000) == 0 && (states & 0b00000001)) states |= 0b00001000; // ONtoOFF = 1

  if (states & 1) states |= 0b00010000; // lastOFF = 1
  else states &= 0b11101111; // lastOFF = 0
}

void Toggle::setStatesTwo() {
  // when using two inputs b6:MIDtoUP, b5:DNtoMID, b4:MIDtoDN, b3:UPtoMID, b2:isDN, b1:isMID, b0:isUP;
  if ((lastRegA & 7) == 6 && (regA & 7) == 4 && (regB & 7) == 7) {
    states |= 0b00000001; // isUP = 1
    states &= 0b11111101; // isMID = 0
    states &= 0b11111011; // isDN = 0
  }
  if ((lastRegA & 7) == 7 && (lastRegB & 7) == 7) {
    states &= 0b11111110; // isUP = 0
    states |= 0b00000010; // isMID = 1
    states &= 0b11111011; // isDN = 0
  }
  if ((lastRegB & 7) == 6 && (regB & 7) == 4 && (regA & 7) == 7) {
    states &= 0b11111110; // isUP = 0
    states &= 0b11111101; // isMID = 0
    states |= 0b00000100; // isDN = 1
  }
  states &= 0b10111111; // MIDtoUP = 0
  if ((lastRegA & 7) == 7 && (regA & 7) == 6) states |= 0b01000000; // MIDtoUP = 1
  states &= 0b11110111; // UPtoMID = 0
  if ((lastRegA & 7) < 7 && (regA & 7) == 7) states |= 0b00001000; // UPtoMID = 1
  states &= 0b11101111; // MIDtoDN = 0
  if ((lastRegB & 7) == 7 && (regB & 7) == 6) states |= 0b00010000; // MIDtoDN = 1
  states &= 0b11011111; // DNtoMID = 0
  if ((lastRegB & 7) < 7 && (regB & 7) == 7) states |= 0b00100000; // DNtoMID = 1
}

//This Port Debouncer uses a robust algorithm that removes spurious signal transitions.
//The algorithm adds only 3 sample periods of time lag to the output signal.
//A 3-sample stable period is required for an output bit to change.
//Therefore, to set an output bit, 3 consecutive 1's are required.
//When 3 consecutive 0's are detected, that bit value is cleared.

uint8_t Toggle::debouncePort() {
  if (_inputMode == inMode::input_port) {
    for (int i = 0; i < 8; i++) {
      if (*_logic & (1 << i) && regA & (1 << i) && lastRegA & (1 << i)) regB |= (1 << i);
      if (!(*_logic & 1 << i) && !(regA & 1 << i) && !(lastRegA & 1 << i)) regB &= ~(1 << i);
    }
    lastRegA = regA;
    regA = *_logic;
    return regB;
  }
    return 0;
}

void Toggle::setInputMode(inMode inputMode) {
  _inputMode = inputMode;
}

void Toggle::setInvertMode(bool invert) {
  if (invert)states |= 0b10000000;
  else states &= 0b01111111;
}

void Toggle::setSampleUs(uint16_t sampleUs) {
  _sampleUs = sampleUs;
}

bool Toggle::getAllTransitions() {
  if (_pinA && _pinB) return states & 0b01111000;
  else if (_pinA) return states & 0b00001100;
  return false;
}

bool Toggle::isOFF() {
  return states & 0b00000001;
}

bool Toggle::isON() {
  return states & 0b00000010;
}

bool Toggle::OFFtoON() {
  return states & 0b00000100;
}

bool Toggle::ONtoOFF() {
  return states & 0b00001000;
}

bool Toggle::isUP() {
  return states & 0b00000001;
}

bool Toggle::isMID() {
  return states & 0b00000010;
}

bool Toggle::isDN() {
  return states & 0b00000100;
}

bool Toggle::UPtoMID() {
  return states & 0b00001000;
}

bool Toggle::MIDtoDN() {
  return states & 0b00010000;
}

bool Toggle::DNtoMID() {
  return states & 0b00100000;
}

bool Toggle::MIDtoUP() {
  return states & 0b01000000;
}
