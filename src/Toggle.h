#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_bit = 250, input_port = 251};

    Toggle();                                        // default constructor must be followed by call to begin in setup
    Toggle(uint8_t *in);
    Toggle(uint8_t inA, uint8_t inB = 255) {
      _inA = inA;
      _inB = inB;
      begin(_inA, _inB);
    };

    void begin(uint8_t inA, uint8_t inB = 255);      // if used in setup, then use default constructor to initialize
    void poll(uint8_t bit = 0);                      // place at top of loop
    uint8_t setAlgorithm(uint8_t glitches = 2);      // (2) robust mode, (1) normal response, (0) quick response
    void setInputMode(inMode inputMode);             // input, input_pullup, input_pulldown, input_bit, input_port
    void setInvertMode(bool invert);                 // invert false: pullup resistors are used, invert true: pulldown resistors
    void setSamplePeriodUs(uint16_t samplePeriodUs); // sample period in microseconds
    uint16_t getElapsedMs();                         // get elapsed ms since the last state change
    uint8_t debounceInput(uint8_t bit = 0);          // the input debouncer
    uint8_t getDebounceCount();                      // counts up to 10 samples for debounce period
    bool toggle(bool invert = false, uint8_t bit = 0); // returns true/false (toggle) at each state change
    bool isPressed(uint8_t bit = 0);                 // returns true if pressed
    bool isReleased(uint8_t bit = 0);                // returns true if released
    bool onPress();                                  // returns true if just pressed
    bool onRelease();                                // returns true if just released
    uint8_t onChange();                              // returns (0) no change, (1) onPress, (2) onRelease
    bool blink(uint16_t ms);                         // returns true for given ms (blink) at each state change
    bool pressedFor(uint16_t ms);                    // returns true if pressed for at least the given ms
    bool releasedFor(uint16_t ms);                   // returns true if released for at least the given ms
    bool retrigger(uint16_t ms);                     // returns true each time the given ms expires while the button is pressed
    bool isUP();                                     // functions for using 2 inputs with 3-position switches
    bool isMID();
    bool isDN();
    bool UPtoMID();
    bool MIDtoDN();
    bool DNtoMID();
    bool MIDtoUP();

  private:

    inMode _inputMode = inMode::input_pullup;        // input mode
    uint8_t _inA, _inB;                              // input pin
    uint8_t *_in;                                    // referenced to input variable
    uint8_t dat, pDat, ppDat;                        // current, previous and 2nd previos input data
    uint16_t _samplePeriodUs = 5000;                 // sample period μs
    uint16_t sampleCount;                            // sample count
    uint32_t sampleUs;                               // sample time μs
    uint8_t out = 0xFF, pOut = 0xFF;                 // debounced output and previous debounced output
    uint8_t csr = 0b10100010;                        // B7-B4: debounceCount, B2: invert, B1-B0 algorithm
    uint8_t lsr = 0b00000000;                        // B5 lastState, B4 toggle, B3 releasedFor, B2 pressedFor, B1 onRelease, B0 onPress

};
#endif
