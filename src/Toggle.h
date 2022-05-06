#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    Toggle(uint8_t pinA, uint8_t pinB = 0);

    void poll();
    bool isOFF();
    bool isON();
    bool OFFtoON();
    bool ONtoOFF();
    bool isUP();
    bool isMID();
    bool isDN();
    bool UPtoMID();
    bool MIDtoDN();
    bool DNtoMID();
    bool MIDtoUP();

  private:

    uint8_t statusOne, statusTwo;
    uint8_t regA = 0xFF, regB = 0xFF, lastRegA = 0xFF, lastRegB = 0xFF;
    uint32_t sampleTime;
    uint8_t _pinA, _pinB;
    bool firstRun = true;
};
#endif
