#pragma once
#ifndef Toggle_h
#define Toggle_h
#include "Arduino.h"

class Toggle {

  public:

    Toggle(uint8_t pinA, uint8_t pinB = 0);
    void poll();

    bool isUP, isMID, isDN, UPtoMID, MIDtoDN, DNtoMID, MIDtoUP, isON, isOFF, ONtoOFF, OFFtoON;

  private:

    uint8_t regA = 0xAA, regB = 0x55, lastRegA = 0xAA, lastRegB = 0x55;
    uint32_t sampleTime, ms;
    uint8_t _pinA, _pinB;
    bool firstRun = true;

};
#endif
