/****************************************************
   Toggle Library for Arduino - Version 2.1.1
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ****************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle(uint8_t pinA, uint8_t pinB)
  : _pinA(pinA), _pinB(pinB)
{
}

void Toggle::poll() {
  if (firstRun) {
    firstRun = false;
    if (_pinA) pinMode(_pinA, INPUT_PULLUP);
    if (_pinB) pinMode(_pinB, INPUT_PULLUP);
  }
  lastRegA = regA;
  lastRegB = regB;

  if (millis() - sampleTime > 10) {
    sampleTime = millis();
    regA = regA << 1 | digitalRead(_pinA);
    if (_pinB) {
      regB = regB << 1 | digitalRead(_pinB);
    }
    else regB = 0xFF;
  }

  // b3:ONtoOFF, b2:OFFtoON, b1:isON, b0:isOFF;

  if (_pinA && !_pinB) {
    if (lastRegA == 0xFF) {
      statusOne |= 0b00000001; // isOFF = 1
      statusOne &= 0b11111101; // isON = 0
    }
    if (regA == 0xFC) {
      statusOne |= 0b00000010; // isON = 1
      statusOne &= 0b11111110; // isOFF = 0
    }
    statusOne &= 0b11111011; // OFFtoON = 0
    if (lastRegA == 0xFF && regA == 0xFE) statusOne |= 0b00000100; // OFFtoON = 1
    statusOne &= 0b11110111; // ONtoOFF = 0
    if (lastRegA == 0x7F && regA == 0xFF) statusOne |= 0b00001000; // ONtoOFF = 1
  }

  // b6:MIDtoUP, b5:DNtoMID, b4:MIDtoDN, b3:UPtoMID, b2:isDN, b1:isMID, b0:isUP;

  if (_pinA && _pinB) {
    if (lastRegA == 0xFE && regA == 0xFC && regB == 0xFF) {
      statusTwo |= 0b00000001; // isUP = 1
      statusTwo &= 0b11111101; // isMID = 0
      statusTwo &= 0b11111011; // isDN = 0
    }
    if (lastRegA == 0xFF && lastRegB == 0xFF) {
      statusTwo &= 0b11111110; // isUP = 0
      statusTwo |= 0b00000010; // isMID = 1
      statusTwo &= 0b11111011; // isDN = 0
    }
    if (lastRegB == 0xFE && regB == 0xFC && regA == 0xFF) {
      statusTwo &= 0b11111110; // isUP = 0
      statusTwo &= 0b11111101; // isMID = 0
      statusTwo |= 0b00000100; // isDN = 1
    }
    statusTwo &= 0b10111111; // MIDtoUP = 0
    if (lastRegA == 0xFF && regA == 0xFE) statusTwo |= 0b01000000; // MIDtoUP = 1
    statusTwo &= 0b11110111; // UPtoMID = 0
    if (lastRegA < 0xFF && regA == 0xFF) statusTwo |= 0b00001000; // UPtoMID = 1
    statusTwo &= 0b11101111; // MIDtoDN = 0
    if (lastRegB == 0xFF && regB == 0xFE) statusTwo |= 0b00010000; // MIDtoDN = 1
    statusTwo &= 0b11011111; // DNtoMID = 0
    if (lastRegB < 0xFF && regB == 0xFF) statusTwo |= 0b00100000; // DNtoMID = 1
  }
}

bool Toggle::isOFF() {
  if ((statusOne & 0b00000001) == 0b00000001) return true;
  return false;
}

bool Toggle::isON() {
  if ((statusOne & 0b00000010) == 0b00000010) return true;
  return false;
}

bool Toggle::OFFtoON() {
  if ((statusOne & 0b00000100) == 0b00000100) return true;
  return false;
}

bool Toggle::ONtoOFF() {
  if ((statusOne & 0b00001000) == 0b00001000) return true;
  return false;
}

bool Toggle::isUP() {
  if ((statusTwo & 0b00000001) == 0b00000001) return true;
  return false;
}

bool Toggle::isMID() {
  if ((statusTwo & 0b00000010) == 0b00000010) return true;
  return false;
}

bool Toggle::isDN() {
  if ((statusTwo & 0b00000100) == 0b00000100) return true;
  return false;
}

bool Toggle::UPtoMID() {
  if ((statusTwo & 0b00001000) == 0b00001000) return true;
  return false;
}

bool Toggle::MIDtoDN() {
  if ((statusTwo & 0b00010000) == 0b00010000) return true;
  return false;
}

bool Toggle::DNtoMID() {
  if ((statusTwo & 0b00100000) == 0b00100000) return true;
  return false;
}

bool Toggle::MIDtoUP() {
  if ((statusTwo & 0b01000000) == 0b01000000) return true;
  return false;
}
