#pragma once
#ifndef Toggle_h
#define Toggle_h
#include <Arduino.h>

class Toggle {

  public:

    enum class inputMode : uint8_t {input = 0, input_pullup = 2, input_pulldown = 9, input_byte = 250};

    Toggle();
    Toggle(uint8_t *in);
    Toggle(uint8_t inA, uint8_t inB = 255) {
      _inA = inA;
      _inB = inB;
    };

    void begin(uint8_t inA, uint8_t inB = 255);        // required in setup
    void poll(uint8_t bit = 0);                        // required at top of loop
    void setInputMode(inputMode inputMode);            // input, input_pullup, input_pulldown, input_byte
    void setInputInvert(bool invert);                  // normal(0), inverted(1)
    bool toggle();                                     // use momentary push button as a toggle switch
    void setToggleState(bool toggleState);             // set toggle state
    void setToggleTrigger(bool change);                // set toggle trigger mode: onPress(0), onRelease(1)
    void setSamplePeriodUs(uint16_t samplePeriodUs);   // sample period in microseconds
    uint32_t getElapsedMs();                           // get elapsed ms since the last state change selected by timer mode
    bool isPressed(uint8_t bit = 0);                   // returns true if pressed
    bool isReleased(uint8_t bit = 0);                  // returns true if released
    bool onPress();                                    // returns true if just pressed
    bool onRelease();                                  // returns true if just released
    uint8_t onChange();                                // returns: no change(0), onPress(1), onRelease(2)
    bool blink(uint16_t ms, uint8_t mode = 1);         // returns true for given ms (blink) on mode: change(0), press(1), release(2),
    bool pressedFor(uint16_t ms);                      // returns true if pressed for at least the given ms
    bool releasedFor(uint16_t ms);                     // returns true if released for at least the given ms
    bool retrigger(uint16_t ms);                       // returns true each time the given ms expires while the button is pressed
    uint8_t pressCode(bool debug = 0);                 // returns byte code for number of fast, short and long clicks
    uint8_t debouncer(uint8_t bit = 0);                // input debouncer
    bool isUP();                                       // functions for using 2 inputs with 3-position switches
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
    inputMode _inputMode = inputMode::input_pullup;

    uint8_t _inA, _inB;                  // input pin
    uint8_t *_in;                        // referenced to input variable
    uint8_t dat, pDat, ppDat;            // current, previous and 2nd previos input data
    uint16_t us_period = 5000;           // sample period μs
    uint32_t startUs = 0;                // for timing transitions
    uint32_t us_timestamp;               // most recent sample time μs
    uint8_t out = 0xFF, pOut = 0xFF;     // debounced output and previous debounced output

    // B7:onPress, B6:onRelease, B5:lastToggleState, B4:toggleState, B3:x, B2:toggleTrigger, B1:invertInput, B0:firstRun
    uint8_t csr = 0b00000001; // status register 

    bool getInputInvert();
    bool getToggleState();
    void setLastToggleState(bool lastToggleState);
    bool getLastToggleState();
    bool getToggleTrigger();
};
#endif
