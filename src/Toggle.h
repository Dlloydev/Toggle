#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_bit = 250, input_port = 251};

    Toggle();
    Toggle(uint8_t inA);
    Toggle(uint8_t inA, uint8_t inB);
    Toggle(uint8_t *in);

    void poll(uint8_t bit = 0);
    bool isOFF(uint8_t bit = 0);
    bool isON(uint8_t bit = 0);
    bool OFFtoON(uint8_t bit = 0);
    bool ONtoOFF(uint8_t bit = 0);
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
    uint8_t setAlgorithm(uint8_t glitches = 2);
    uint8_t debounceInput(uint8_t bit = 0);
    uint8_t samples();

  private:
    void init();

    inMode _inputMode = inMode::input_pullup;  // input mode
    uint8_t _inA, _inB;                        // arduino input pin
    uint8_t *_in;                              // referenced to input variable
    uint8_t dat, pDat, ppDat;                  // current, previous and 2nd previos input data
    uint16_t _sampleUs = 5000;                 // sample period μs
    uint32_t lastUs;                           // last sample μs
    uint8_t out = 0xFF, pOut = 0xFF;           // debounced output and previous debounced output
    uint8_t csr = 0b10101010;                  // B7-B4: samples B3: first run, B2: invert, B1-B0 algorithm

};
#endif
