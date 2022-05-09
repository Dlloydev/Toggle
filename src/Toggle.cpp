/****************************************************
   Toggle Library for Arduino - Version 2.2.0
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

    if (_pinA) {
      if (_inputMode == static_cast<uint8_t>(inMode::input_pullup)) pinMode(_pinA, INPUT_PULLUP);
      else if (_inputMode == static_cast<uint8_t>(inMode::input)) pinMode(_pinA, INPUT);
    }

    if (_pinB) {
      if (_inputMode == static_cast<uint8_t>(inMode::input_pullup)) pinMode(_pinB, INPUT_PULLUP);
      else if (_inputMode == static_cast<uint8_t>(inMode::input)) pinMode(_pinB, INPUT);
    }
  }
  lastRegA = regA;
  lastRegB = regB;

  if (millis() - sampleTime > 5) {
    sampleTime = millis();
    if (_inputMode <= 2) {
      regA = regA << 1 | digitalRead(_pinA);
      if (_pinB) regB = regB << 1 | digitalRead(_pinB);
    }
    if (_inputMode == static_cast<uint8_t>(inMode::input_logic)) {
      uint8_t tmp = *_logic;
      regA = regA << 1 | tmp;
    }
  }
  if (_pinA && _pinB) setStatesTwo();
  if (_pinA || _inputMode == static_cast<uint8_t>(inMode::input_logic)) setStatesOne();
}

void Toggle::setStatesOne() {
  // when using one input b3:ONtoOFF, b2:OFFtoON, b1:isON, b0:isOFF;
  if (lastRegA == 0xFF) {
    statesOne |= 0b00000001; // isOFF = 1
    statesOne &= 0b11111101; // isON = 0
  }
  if ((regA & 3) == 0) {
    statesOne |= 0b00000010; // isON = 1
    statesOne &= 0b11111110; // isOFF = 0
  }
  statesOne &= 0b11111011; // OFFtoON = 0
  //if (lastRegA == 0xFF && regA == 0xFE) statesOne |= 0b00000100; // OFFtoON = 1
  if ((lastRegA & 1) == 0 && (regA & 3) == 0) statesOne |= 0b00000100; // OFFtoON = 1
  statesOne &= 0b11110111; // ONtoOFF = 0
  if (lastRegA == 0x7F && regA == 0xFF) statesOne |= 0b00001000; // ONtoOFF = 1
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
  _inputMode = static_cast<uint8_t>(inputMode);
}

uint8_t Toggle::getInputMode() {
  return static_cast<uint8_t>(_inputMode);
}

bool Toggle::isOFF() {
  if ((statesOne & 0b00000001) == 0b00000001) return true;
  return false;
}

bool Toggle::isON() {
  if ((statesOne & 0b00000010) == 0b00000010) return true;
  return false;
}

bool Toggle::OFFtoON() {
  if ((statesOne & 0b00000100) == 0b00000100) return true;
  return false;
}

bool Toggle::ONtoOFF() {
  if ((statesOne & 0b00001000) == 0b00001000) return true;
  return false;
}

bool Toggle::isUP() {
  if ((statesTwo & 0b00000001) == 0b00000001) return true;
  return false;
}

bool Toggle::isMID() {
  if ((statesTwo & 0b00000010) == 0b00000010) return true;
  return false;
}

bool Toggle::isDN() {
  if ((statesTwo & 0b00000100) == 0b00000100) return true;
  return false;
}

bool Toggle::UPtoMID() {
  if ((statesTwo & 0b00001000) == 0b00001000) return true;
  return false;
}

bool Toggle::MIDtoDN() {
  if ((statesTwo & 0b00010000) == 0b00010000) return true;
  return false;
}

bool Toggle::DNtoMID() {
  if ((statesTwo & 0b00100000) == 0b00100000) return true;
  return false;
}

bool Toggle::MIDtoUP() {
  if ((statesTwo & 0b01000000) == 0b01000000) return true;
  return false;
}
