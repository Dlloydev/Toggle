/****************************************************
   Toggle Library for Arduino - Version 2.2.1
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
    statesOne |= 0b00010000; // lastOFF = 1

    if (_pinA) {
      if (_inputMode == inMode::input_pullup) pinMode(_pinA, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_pinA, INPUT);
    }
    if (_pinB) {
      if (_inputMode == inMode::input_pullup) pinMode(_pinB, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_pinB, INPUT);
    }
  }
  lastRegA = regA;
  lastRegB = regB;
  if (micros() - lastUs > _sampleUs) {
    lastUs += _sampleUs;
    if (_inputMode == inMode::input || _inputMode == inMode::input_pullup) {
      regA = regA << 1 | digitalRead(_pinA);
      if (_pinB) regB = regB << 1 | digitalRead(_pinB);
    }
    if (_inputMode == inMode::input_logic) {
      uint8_t tmp = *_logic;
      regA = regA << 1 | tmp;
    }
  }
  if (_pinA && _pinB) setStatesTwo();
  if (_pinA || _inputMode == inMode::input_logic) setStatesOne();
}

void Toggle::setStatesOne() {
  // when using one input b4:lastOFF, b3:ONtoOFF, b2:OFFtoON, b1:isON, b0:isOFF;
  if (lastRegA == 0xFF) {
    statesOne |= 0b00000001; // isOFF = 1
    statesOne &= 0b11111101; // isON = 0
  }
  if ((regA & 3) == 0) {
    statesOne |= 0b00000010; // isON = 1
    statesOne &= 0b11111110; // isOFF = 0
  }
  statesOne &= 0b11111011; // OFFtoON = 0
  if ((statesOne & 0b00010000) && (statesOne & 0b00000001) == 0) statesOne |= 0b00000100; // OFFtoON = 1

  statesOne &= 0b11110111; // ONtoOFF = 0
  if ((statesOne & 0b00010000) == 0 && (statesOne & 0b00000001)) statesOne |= 0b00001000; // ONtoOFF = 1

  if (statesOne & 1) statesOne |= 0b00010000; // lastOFF = 1
  else statesOne &= 0b11101111; // lastOFF = 0
}

void Toggle::setStatesTwo() {
  // when using two inputs b6:MIDtoUP, b5:DNtoMID, b4:MIDtoDN, b3:UPtoMID, b2:isDN, b1:isMID, b0:isUP;
  if (lastRegA == 0xFE && regA == 0xFC && regB == 0xFF) {
    statesTwo |= 0b00000001; // isUP = 1
    statesTwo &= 0b11111101; // isMID = 0
    statesTwo &= 0b11111011; // isDN = 0
  }
  if (lastRegA == 0xFF && lastRegB == 0xFF) {
    statesTwo &= 0b11111110; // isUP = 0
    statesTwo |= 0b00000010; // isMID = 1
    statesTwo &= 0b11111011; // isDN = 0
  }
  if (lastRegB == 0xFE && regB == 0xFC && regA == 0xFF) {
    statesTwo &= 0b11111110; // isUP = 0
    statesTwo &= 0b11111101; // isMID = 0
    statesTwo |= 0b00000100; // isDN = 1
  }
  statesTwo &= 0b10111111; // MIDtoUP = 0
  if (lastRegA == 0xFF && regA == 0xFE) statesTwo |= 0b01000000; // MIDtoUP = 1
  statesTwo &= 0b11110111; // UPtoMID = 0
  if (lastRegA < 0xFF && regA == 0xFF) statesTwo |= 0b00001000; // UPtoMID = 1
  statesTwo &= 0b11101111; // MIDtoDN = 0
  if (lastRegB == 0xFF && regB == 0xFE) statesTwo |= 0b00010000; // MIDtoDN = 1
  statesTwo &= 0b11011111; // DNtoMID = 0
  if (lastRegB < 0xFF && regB == 0xFF) statesTwo |= 0b00100000; // DNtoMID = 1
}

void Toggle::setInputMode(inMode inputMode) {
  _inputMode = inputMode;
}

void Toggle::setSampleUs(uint16_t sampleUs) {
  _sampleUs = sampleUs;
}

bool Toggle::isOFF() {
  return statesOne & 0b00000001;
}

bool Toggle::isON() {
  return statesOne & 0b00000010;
}

bool Toggle::OFFtoON() {
  return statesOne & 0b00000100;
}

bool Toggle::ONtoOFF() {
  return statesOne & 0b00001000;
}

bool Toggle::isUP() {
  return statesTwo & 0b00000001;
}

bool Toggle::isMID() {
  return statesTwo & 0b00000010;
}

bool Toggle::isDN() {
  return statesTwo & 0b00000100;
}

bool Toggle::UPtoMID() {
  return statesTwo & 0b00001000;
}

bool Toggle::MIDtoDN() {
  return statesTwo & 0b00010000;
}

bool Toggle::DNtoMID() {
  return statesTwo & 0b00100000;
}

bool Toggle::MIDtoUP() {
  return statesTwo & 0b01000000;
}
