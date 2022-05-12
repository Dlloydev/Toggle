#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_logic = 250, input_portA = 251, input_portB = 252};

    Toggle(uint8_t pinA);
    Toggle(uint8_t pinA, uint8_t pinB);
    Toggle(uint8_t *inA);
    Toggle(uint8_t *inA, uint8_t *inB);

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
    bool getAllTransitions();
    void setInputMode(inMode inputMode);
    void setInvertMode(bool invert);
    void setSampleUs(uint16_t sampleUs);
    uint8_t debouncePortA();
    uint8_t debouncePortB();

  private:
    void setStatesOne();
    void setStatesTwo();

    inMode _inputMode = inMode::input_pullup;
    uint8_t _pinA, _pinB;
    uint8_t *_inA, *_inB;

    bool firstRun = true;
    uint8_t states = 0b00010000, regA = 7, regB = 7, lastRegA = 7, lastRegB = 7;
    uint16_t _sampleUs = 5000;
    uint32_t lastUs;

};
#endif
