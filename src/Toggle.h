#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_port = 250};

    Toggle(uint8_t inA);
    Toggle(uint8_t inA, uint8_t inB);
    Toggle(uint8_t *in);

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
    void setInvertMode(bool invert);
    void setSampleUs(uint16_t sampleUs);
    uint8_t debouncePort();

  private:
    void init();

    inMode _inputMode = inMode::input_pullup;
    uint8_t _inA, _inB, dat, pDat, ppDat;
    uint8_t *_in;
    uint8_t states = 0b10000000, out = 0xFF, pOut = 0xFF;
    uint16_t _sampleUs = 5000;
    uint32_t lastUs;

};
#endif
