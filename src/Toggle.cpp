/************************************************
   Toggle Library for Arduino - Version 2.5.3
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle() { }
Toggle::Toggle(uint8_t inA) : _inA(inA) { }
Toggle::Toggle(uint8_t inA, uint8_t inB) : _inA(inA), _inB(inB) { }
Toggle::Toggle(uint8_t *in) : _in(in) { }

void Toggle::poll(uint8_t bit) {
  init();
  if (micros() - lastUs > _sampleUs) {
    lastUs += _sampleUs;
    if (_inputMode == inMode::input || _inputMode == inMode::input_pullup || _inputMode == inMode::input_pulldown) {
      if (_inA) dat = digitalRead(_inA);
      if (_inB) dat += digitalRead(_inB) * 2;
    }
    if (_inputMode == inMode::input_bit || _inputMode == inMode::input_port) dat = *_in;
    if (csr & 0b00000100) dat = ~dat; // if invert mode
    if (bit > 7) bit = 0;
    debounceInput(bit);
    if ((csr & 0xF0) < 0xA0) csr += 0x10; // if samples < 10, samples++;
  } // run sample
} // poll

uint8_t Toggle::setAlgorithm(uint8_t glitches) {
  csr |=  0b00000010;    // 2 glitches ignored (default)
  csr &= ~0b00000001;
  if (glitches == 1) {  // 1 glitch ignored
    csr |=  0b00000001;
    csr &= ~0b00000010;
  }
  else if (glitches == 0) csr &= ~0b00000011;  // 0 glitches ignored
  return csr;
}

/*
  The debounceInput() function by default uses a robust algorithm that removes several spurious
  signal transitions a(glitches) nd only adds 2 sample periods time lag to the output signal.
  A 3-sample stable period is required for an output bit to change. Optionally, this can be
  changed to 1 glitch removal with 1 sample period time lag or to immediate response mode.
*/
uint8_t Toggle::debounceInput(uint8_t bit) {
  pOut = out;
  uint8_t bits = 2;
  if (_inputMode == inMode::input_bit) bits = 1;
  if (_inputMode == inMode::input_port) bits = 8;
  for (int i = bit; i < bit + bits; i++) {

    if (csr & 0b00000010) { // 2 glitches ignored
      if (dat & (1 << i) && pDat & (1 << i) && ppDat & (1 << i)) out |= (1 << i);
      else if (!(dat & 1 << i) && !(pDat & 1 << i) && !(ppDat & 1 << i)) out &= ~(1 << i);

    } else if (csr & 0b00000001) { // 1 glitch ignored
      if (dat & (1 << i) && pDat & (1 << i)) out |= (1 << i);
      else if (!(dat & 1 << i) && !(pDat & 1 << i)) out &= ~(1 << i);

    } else { // immediate ON response, delayed OFF
      if ((csr & 0xF0) == 0xA0) { // if samples == 10
        if (dat & (1 << i)) out |= (1 << i);
        else if (!(dat & 1 << i)) out &= ~(1 << i);
      }
    }
  }
  ppDat = pDat;
  pDat = dat;
  return out;
}

bool Toggle::isOFF(uint8_t bit) {
  return out & (1 << bit);
}

bool Toggle::isON(uint8_t bit) {
  return !isOFF(bit);
}

bool Toggle::OFFtoON(uint8_t bit) {
  if ((pOut & (1 << bit)) && !isOFF(bit)) {
    if ((csr & 3) == 0 && (csr & 0xF0) == 0xA0) csr &= ~0xF0; // samples = 0;
      pOut = out;
      return true;
    }
  return false;
}

bool Toggle::ONtoOFF(uint8_t bit) {
  if ((!(pOut & (1 << bit))) && isOFF(bit)) {
    if ((csr & 3) == 0 && (csr & 0xF0) == 0xA0) csr &= ~0xF0; // samples = 0;
      pOut = out;
      return true;
    }
  return false;
}

uint8_t Toggle::samples() {
  return csr >> 4;
}

bool Toggle::isUP() {
  return isON();
}

bool Toggle::isDN() {
  return !(out & 0b00000010);
}
bool Toggle::isMID() {
  return isOFF() && !isDN();
}

bool Toggle::UPtoMID() {
  return ONtoOFF();
}

bool Toggle::MIDtoUP() {
  return OFFtoON();
}

bool Toggle::MIDtoDN() {
  if ((pOut & 0b00000010) && isDN()) {
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::DNtoMID() {
  if (!(pOut & 0b00000010) && !isDN()) {
    pOut = out;
    return true;
  }
  return false;
}

void Toggle::setInputMode(inMode inputMode) {
  _inputMode = inputMode;
}

void Toggle::setInvertMode(bool invert) {
  if (invert)csr |= 0b00000100; //set
  else csr &= ~0b00000100; //clear
}

void Toggle::setSampleUs(uint16_t sampleUs) {
  _sampleUs = sampleUs;
}

void Toggle::init() {
  if (csr & 0b00001000) { // first run
    csr &= ~0b00001000; // clear first run
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
    if (_inputMode == inMode::input_bit || _inputMode == inMode::input_port) {
      _inA = 0;
      _inB = 0;
    } else {
      *_in = 0;
      _in = 0;
    }
  }
}
