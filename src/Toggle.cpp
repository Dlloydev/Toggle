/************************************************
   Toggle Library for Arduino - Version 3.1.0
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle() : _inA(255), _inB(255) {}
Toggle::Toggle(uint8_t *in) : _in(in) {}

void Toggle::begin(uint8_t inA, uint8_t inB) {
  if (csr & 0b00001000) { // first run
    csr &= ~0b00001000;   // clear first run
    _inA = inA;
    _inB = inB;
    us_timestamp = micros();
    startUs = us_timestamp;
    if (_inA != 255) {
      if (_inputMode == inMode::input_pullup) pinMode(_inA, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_inA, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_inA, INPUT_PULLDOWN);
#endif
    }
    if (_inB != 255) {
      if (_inputMode == inMode::input_pullup) pinMode(_inB, INPUT_PULLUP);
      else if (_inputMode == inMode::input) pinMode(_inB, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inMode::input_pulldown) pinMode(_inB, INPUT_PULLDOWN);
#endif
    }
  }
}

void Toggle::poll(uint8_t bit) {
  begin(_inA, _inB); // runs one only
  if (micros() - us_timestamp > us_period) {
    us_timestamp += us_period;
    if (_inputMode == inMode::input || _inputMode == inMode::input_pullup || _inputMode == inMode::input_pulldown) {
      if (_inA) dat = digitalRead(_inA);
      if (_inB) dat += digitalRead(_inB) * 2;
    }
    if (_inputMode == inMode::input_bit || _inputMode == inMode::input_port) dat = *_in;
    if (csr & 0b00000100) dat = ~dat; // if invert mode
    if (bit > 7) bit = 0;
    debounceInput(bit);
    if ((csr & 0xF0) < 0xA0) csr += 0x10; // increment debounceCount (10 max)
  } // run sample
} // poll

uint8_t Toggle::setAlgorithm(uint8_t glitches) {
  csr |=  0b00000010;    // 2 glitches ignored (default)
  csr &= ~0b00000001;
  if (glitches == 1) {  // 1 glitch ignored
    csr |=  0b00000001;
    csr &= ~0b00000010;
  }
  else if (glitches == 0) csr &= ~0b00000011;  // 0 glitches ignored
  return csr;
}

void Toggle::setInputMode(inMode inputMode) {
  _inputMode = inputMode;
}

void Toggle::setInvertMode(bool invert) {
  if (invert)csr |= 0b00000100; //set
  else csr &= ~0b00000100; //clear
}

void Toggle::setSamplePeriodUs(uint16_t period) {
  us_period = period;
}

/************* button state functions ****************/

bool Toggle::isPressed(uint8_t bit) {
  return !isReleased(bit);
}

bool Toggle::isReleased(uint8_t bit) {
  return out & (1 << bit);
}

bool Toggle::onPress() {
  if (lsr & 0b00000001) { // onPress
    if ((csr & 3) == 0 && (csr & 0xF0) == 0xA0) csr &= ~0xF0; // debounceCount = 0;;
    lsr &=  ~0b00000001; // clear onPress flag
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::onRelease() {
  if (lsr & 0b00000010) { // onRelease
    if ((csr & 3) == 0 && (csr & 0xF0) == 0xA0) csr &= ~0xF0; // debounceCount = 0;
    lsr &=  ~0b00000010; // clear onRelease flag
    pOut = out;
    return true;
  }
  return false;
}

uint8_t Toggle::onChange() {
  if (lsr & 0b00000010) return 2; // onRelease
  else if (lsr & 0b00000001) return 1; // onPress
  return 0;
}

bool Toggle::toggle(bool invert, uint8_t bit) {
  if (isPressed(bit) && !(lsr & 0b00100000)) {
    lsr |= 0b00100000; // set lastState
    (lsr & 0b00010000) ? lsr &= ~0b00010000 : lsr |= 0b00010000; // negate toggleFlag
  }
  if (isReleased(bit) && (lsr & 0b00100000)) lsr &= ~0b00100000; // clear lastState
  return (invert) ? !(lsr & 0b00010000) : (lsr & 0b00010000);
}

/************* button timer functions ****************/

void Toggle::setTimerMode(uint8_t mode) {
  lsr &= ~0b11000000; // onPress (default)
  if (mode == 1) lsr |=  0b01000000; // onRelease
  else if (mode == 2) { // onChange
    lsr &= ~0b01000000;
    lsr |=  0b10000000;
  }
}

uint8_t Toggle::getTimerMode() {
  return (lsr & 0b11000000) >> 6;
}

bool Toggle::blink(uint16_t ms) {
  return (bool)(ms > (getElapsedMs()));
}

bool Toggle::pressedFor(uint16_t ms) {
  if (getTimerMode()) setTimerMode(0); // start onPress
  if (isPressed() && getElapsedMs() > ms) {
    return true;
  }
  return false;
}

bool Toggle::releasedFor(uint16_t ms) {
  if (getTimerMode() != 1) setTimerMode(1); // start onRelease
  if (isReleased() && getElapsedMs() > ms) {
    return true;
  }
  return false;
}

bool Toggle::retrigger(uint16_t ms) {
  if (getTimerMode()) setTimerMode(0); // start onPress
  if (isPressed() && getElapsedMs() > ms) {
    //clearTimer();
    return true;
  }
  return false;
}

uint32_t Toggle::getElapsedMs() {
  return (micros() - startUs) * 0.001;
}

uint8_t Toggle::pressCode() {
  static uint8_t pCode = 0, code = 0;
  static uint32_t elapsedMs = 0;

  //Serial.print(F(" startUs: ")); Serial.print(startUs); Serial.print(F("  "));
  //Serial.print(F(" elapsedMS: ")); Serial.print(getElapsedMs()); Serial.print(F("  "));
  //Serial.print(F(" pCode: ")); Serial.print(pCode, HEX); Serial.print(F("  "));

  switch (_state) {
    case PB_DEFAULT:
      setTimerMode(2); // onChange
      elapsedMs = getElapsedMs();
      if (pCode && (elapsedMs > CLICK::LONG)) _state = PB_DONE;
      if (onChange()) startUs = micros();
      if (onPress()) {
        //Serial.print(F(" releasedMs: ")); Serial.print(elapsedMs); Serial.print(F("  "));
        _state = PB_ON_PRESS;
      }
      if (onRelease()) {
        //Serial.print(F(" pressedMs: ")); Serial.print(elapsedMs); Serial.print(F("  "));
        _state = PB_ON_RELEASE;
      }
      break;

    case PB_ON_PRESS:
      _state = PB_DEFAULT;
      break;

    case PB_ON_RELEASE:
      if ((elapsedMs < CLICK::MULTI) && (!pCode || (pCode > 0xEF))) _state = PB_MULTI_CLICKS;
      else if (elapsedMs < CLICK::LONG) _state = PB_SHORT_CLICKS;
      else _state = PB_LONG_CLICKS;
      break;

    case PB_MULTI_CLICKS:
      pCode |= 0xF0;
      if ((pCode & 0x0F) < 0x0F) pCode += 1;
      _state = PB_DEFAULT;
      break;

    case PB_SHORT_CLICKS:
      if ((pCode & 0x0F) < 0x0F) pCode += 1;
      _state = PB_DEFAULT;
      break;

    case PB_LONG_CLICKS:
      if ((pCode & 0xE0) < 0xE0) pCode += 0x10;
      _state = PB_DEFAULT;
      break;

    case PB_DONE:
      code = pCode;
      pCode = 0;
      _state = PB_DEFAULT;
      return code;
      break;

    default:
      _state = PB_DEFAULT;
      break;
  }
  return code;
}

/************** debouncer ***********************************************************************
  The debounceInput() function by default uses a robust algorithm that ignores up to 2 spurious
  signal transitions (glitches) and only adds up to 2 sample periods time lag to the output signal.
  Optionally, this can be changed to 1 glitch with 1 sample lag or to immediate response mode.
*************************************************************************************************/
uint8_t Toggle::debounceInput(uint8_t bit) {
  pOut = out;
  uint8_t a, b, c, bits = 2;
  if (_inputMode == inMode::input_bit) bits = 1;
  if (_inputMode == inMode::input_port) bits = 8;

  for (int i = bit; i < bit + bits; i++) {
    a = dat & (1 << i);
    b = pDat & (1 << i);
    c = ppDat & (1 << i);

    if (csr & 0b00000010) { // 2 glitches ignored
      if (a && b && c) out |= (1 << i);
      else if (!a && !b && !c) out &= ~(1 << i);

    } else if (csr & 0b00000001) { // 1 glitch ignored
      if (a && b) out |= (1 << i);
      else if (!a && !b) out &= ~(1 << i);

    } else { // immediate ON response, delayed OFF
      if ((csr & 0xF0) == 0xA0) { // if samples == 10
        if (a) out |= (1 << i);
        else if (!a) out &= ~(1 << i);
      }
    }
  }
  if ((pOut & (1 << bit)) && !isReleased(bit)) {
    lsr |=  0b00000001; // set onPress
    lsr &= ~0b00000010; // clear onRelease
  } else {
    if ((!(pOut & (1 << bit))) && isReleased(bit)) {
      lsr |=  0b00000010; // set onRelease
      lsr &= ~0b00000001; // clear onPress
    }
  }
  ppDat = pDat;
  pDat = dat;
  return out;
}

/*********** 3 position switch functions ****************/

bool Toggle::isUP() {
  return isPressed();
}

bool Toggle::isDN() {
  return !(out & 0b00000010);
}
bool Toggle::isMID() {
  return isReleased() && !isDN();
}

bool Toggle::UPtoMID() {
  return onRelease();
}

bool Toggle::MIDtoUP() {
  return onPress();
}

bool Toggle::MIDtoDN() {
  if ((pOut & 0b00000010) && isDN()) {
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::DNtoMID() {
  if (!(pOut & 0b00000010) && !isDN()) {
    pOut = out;
    return true;
  }
  return false;
}
