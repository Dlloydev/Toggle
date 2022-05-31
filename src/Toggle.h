#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_bit = 250, input_port = 251};

    Toggle();
    Toggle(uint8_t *in);
    Toggle(uint8_t inA, uint8_t inB = 255) {
      _inA = inA;
      _inB = inB;
    };

    void begin(uint8_t inA, uint8_t inB = 255);      // required in setup
    void poll(uint8_t bit = 0);                      // required at top of loop

    uint8_t setAlgorithm(uint8_t glitches = 2);      // (2) robust mode, (1) average mode, (0) common mode
    void setInputMode(inMode inputMode);             // input, input_pullup, input_pulldown, input_bit, input_port
    void setInvertMode(bool invert);                 // invert false: pullup resistors are used, invert true: pulldown resistors
    void setSamplePeriodUs(uint16_t samplePeriodUs); // sample period in microseconds
    uint32_t getElapsedMs();                         // get elapsed ms since the last state change selected by timer mode

    bool isPressed(uint8_t bit = 0);                 // returns true if pressed
    bool isReleased(uint8_t bit = 0);                // returns true if released
    bool onPress();                                  // returns true if just pressed
    bool onRelease();                                // returns true if just released
    uint8_t onChange();                              // returns (0) no change, (1) onPress, (2) onRelease
    bool toggle(bool invert = 0, uint8_t bit = 0);   // returns true/false (toggle) on each press
    bool blink(uint16_t ms, uint8_t mode = 0);       // returns true for given ms (blink) on mode press(0), release(1), change(2)
    bool pressedFor(uint16_t ms);                    // returns true if pressed for at least the given ms
    bool releasedFor(uint16_t ms);                   // returns true if released for at least the given ms
    bool retrigger(uint16_t ms);                     // returns true each time the given ms expires while the button is pressed
    uint8_t pressCode(bool debug = 0);               // returns byte code for number of fast, short and long clicks
    uint8_t debounceInput(uint8_t bit = 0);          // input debouncer

    bool isUP();                                     // functions for using 2 inputs with 3-position switches
    bool isMID();
    bool isDN();
    bool UPtoMID();
    bool MIDtoDN();
    bool DNtoMID();
    bool MIDtoUP();

  private:

    enum CLICK : uint16_t {MULTI = 400, LONG = 1000};
    enum fsm_t : uint8_t {  // finite state machine
      PB_DEFAULT = 0,
      PB_ON_PRESS = 1,
      PB_ON_RELEASE = 2,
      PB_MULTI_CLICKS = 3,
      PB_SHORT_CLICKS = 4,
      PB_LONG_CLICKS = 5,
      PB_DONE = 6
    };

    fsm_t _state = PB_DEFAULT;

    inMode _inputMode = inMode::input_pullup;        // input mode
    uint8_t _inA, _inB;                              // input pin
    uint8_t *_in;                                    // referenced to input variable
    uint8_t dat, pDat, ppDat;                        // current, previous and 2nd previos input data
    uint16_t us_period = 5000;                       // sample period μs
    uint32_t startUs = 0;                            // for timing transitions
    uint32_t us_timestamp;                           // most recent sample time μs
    uint8_t out = 0xFF, pOut = 0xFF;                 // debounced output and previous debounced output
    uint8_t csr = 0b10101010;                        // B7-B4: debounceCount, B3: first run B2: invert, B1-B0 algorithm
    uint8_t lsr = 0b00000000;                        // B5 lastState, B4 toggle, B1 onRelease, B0 onPress

};
#endif
