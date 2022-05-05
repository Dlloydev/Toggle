/****************************************************
   Toggle Library for Arduino - Version 2.0.0
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
    if (_pinB) {
      pinMode(_pinB, INPUT_PULLUP);
    } else {
      lastRegA = 0xFF;
      regA = 0xFF;
    }
  }
  ms = millis();

  lastRegA = regA;
  lastRegB = regB;

  if (ms - sampleTime > 10) {
    regA = regA << 1 | digitalRead(_pinA);
    if (_pinB) {
      regB = regB << 1 | digitalRead(_pinB);
    }
    else regB = 0xFF;
    sampleTime = ms;
  }

  if (_pinA && !_pinB) {
    if (regA == 0xFF) {
      isOFF = true;
      isON = false;
    }
    if (regA == 0xFC) {
      isON = true;
      isOFF = false;
    }
    OFFtoON = false;
    if (lastRegA == 0xFE && regA == 0xFC) OFFtoON = true;
    ONtoOFF = false;
    if (lastRegA == 0x7F && regA == 0xFF) ONtoOFF = true;
  }

  if (_pinA && _pinB) {
    if (lastRegA == 0xFE && regA == 0xFC && regB == 0xFF) {
      isUP = true;
      isMID = false;
      isDN = false;
    }
    if (regA == 0xFF && regB == 0xFF) {
      isUP = false;
      isMID = true;
      isDN = false;
    }
    if (lastRegB == 0xFE && regB == 0xFC && regA == 0xFF) {
      isUP = false;
      isMID = false;
      isDN = true;
    }
    UPtoMID = false;
    if (lastRegA == 0x1 && regA == 0x3 && regB == 0xFF) UPtoMID = true;
    MIDtoDN = false;
    if (lastRegB == 0xFE && regB == 0xFC && regA == 0xFF) MIDtoDN = true;
    DNtoMID = false;
    if (lastRegB == 0x1 && regB == 0x3 && regA == 0xFF) DNtoMID = true;
    MIDtoUP = false;
    if (lastRegA == 0xFE && regA == 0xFC && regB == 0xFF) MIDtoUP = true;
  }
}
