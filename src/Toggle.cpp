/************************************************
   Toggle Library for Arduino - Version 2.5.1
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle(uint8_t inA) : _inA(inA) { }
Toggle::Toggle(uint8_t inA, uint8_t inB) : _inA(inA), _inB(inB) { }
Toggle::Toggle(uint8_t *in) : _in(in) { }

void Toggle::poll() {
  init();
  if (micros() - lastUs > _sampleUs) {
    lastUs += _sampleUs;
    if (_inputMode == inMode::input || _inputMode == inMode::input_pullup || _inputMode == inMode::input_pulldown) {
      if (_inA) dat = digitalRead(_inA);
      if (_inB) dat += digitalRead(_inB) * 2;
    }
    if (_inputMode == inMode::input_port) dat = *_in;
    if (states & 0b01000000) dat = ~dat; //bitwise not
    debouncePort();
  } //run sample
} //poll
/*
  The debouncePort() function uses a robust algorithm that removes spurious
  signal transitions and only adds 2 sample periods time lag to the output
  signal. A 3-sample stable period is required for an output bit to change.
  Each output bit is set after 3 consecutive 1's are detected and cleared
  after 3 consecutive 0's are detected.
*/
uint8_t Toggle::debouncePort() {
  pOut = out;
  uint8_t bits = 2;
  if (_inputMode == inMode::input_port) bits = 8;
  for (int i = 0; i < bits; i++) {
    if (dat & (1 << i) && pDat & (1 << i) && ppDat & (1 << i)) out |= (1 << i);
    else if (!(dat & 1 << i) && !(pDat & 1 << i) && !(ppDat & 1 << i)) out &= ~(1 << i);
  }
  ppDat = pDat;
  pDat = dat;
  return out;
}

bool Toggle::isOFF() {
  return out & 0b00000001;
}

bool Toggle::isON() {
  return !(out & 0b00000001);
}

bool Toggle::OFFtoON() {
  if ((pOut & 0b00000001) && !(out & 0b00000001)) {
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::ONtoOFF() {
  if (!(pOut & 0b00000001) && (out & 0b00000001)) {
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::isUP() {
  return isON();
}

bool Toggle::isMID() {
  return (out & 0b00000001) && (out & 0b00000010);
}

bool Toggle::isDN() {
  return !(out & 0b00000010);
}

bool Toggle::UPtoMID() {
  return ONtoOFF();
}

bool Toggle::MIDtoUP() {
  return OFFtoON();
}

bool Toggle::MIDtoDN() {
  if ((pOut & 0b00000010) && !(out & 0b00000010)) {
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::DNtoMID() {
  if (!(pOut & 0b00000010) && (out & 0b00000010)) {
    pOut = out;
    return true;
  }
  return false;
}

void Toggle::setInputMode(inMode inputMode) {
  _inputMode = inputMode;
}

void Toggle::setInvertMode(bool invert) {
  if (invert)states |= 0b01000000;
  else states &= 0b10111111;
}

void Toggle::setSampleUs(uint16_t sampleUs) {
  _sampleUs = sampleUs;
}

void Toggle::init() {
  if (states & 0b10000000) { //first run
    states &= 0b01111111; //clear first run
    lastUs = micros();
    if (_inA) {
      if (_inputMode == inMode::input_pullup) pinMode(_inA, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_inA, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_inA, INPUT_PULLDOWN);
#endif
    }
    if (_inB) {
      if (_inputMode == inMode::input_pullup) pinMode(_inB, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_inB, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_inB, INPUT_PULLDOWN);
#endif
    }
    if (_inputMode == inMode::input_port) {
      _inA = 0;
      _inB = 0;
    } else {
      *_in = 0;
      _in = 0;
    }
  }
}
