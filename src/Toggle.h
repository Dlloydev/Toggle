#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9,  input_logic = 250};

    Toggle(uint8_t pinA);
    Toggle(uint8_t pinA, uint8_t pinB);
    Toggle(uint8_t *logic);

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
    void setInputMode(inMode inputMode);
    void setSampleUs(uint16_t sampleUs);

   private:
    void setStatesOne();
    void setStatesTwo();

    inMode _inputMode = inMode::input_pullup;
    uint8_t _pinA, _pinB;
    uint8_t *_logic;
 
    bool firstRun = true;
    uint8_t regA = 0xFF, regB = 0xFF, lastRegA = 0xFF, lastRegB = 0xFF;
    uint8_t statesOne, statesTwo;
    uint16_t _sampleUs = 5000;
    uint32_t lastUs;

};
#endif
