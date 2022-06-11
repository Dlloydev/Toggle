/************************************************
   Toggle Library for Arduino - Version 3.1.6
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ************************************************/

#include "Toggle.h"
#include <Arduino.h>

Toggle::Toggle() : _inA(255), _inB(255) {}
Toggle::Toggle(uint8_t *in) : _in(in) {}

void Toggle::begin(uint8_t inA, uint8_t inB) {
  if (csr & 0b00000001) { // firstRun
    csr &= ~0b00000001;   // clear firstRun
    _inA = inA;
    _inB = inB;
    us_timestamp = micros();
    startUs = us_timestamp;
    if (_inA != 255) {
      if (_inputMode == inputMode::input_pullup) pinMode(_inA, INPUT_PULLUP);
      else if (_inputMode == inputMode::input) pinMode(_inA, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inputMode::input_pulldown) pinMode(_inA, INPUT_PULLDOWN);
#endif
    }
    if (_inB != 255) {
      if (_inputMode == inputMode::input_pullup) pinMode(_inB, INPUT_PULLUP);
      else if (_inputMode == inputMode::input) pinMode(_inB, INPUT);
#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
      else if (_inputMode == inputMode::input_pulldown) pinMode(_inB, INPUT_PULLDOWN);
#endif
    }
  }
}

void Toggle::poll(uint8_t bit) {
  begin(_inA, _inB); // runs one only
  if (micros() - us_timestamp > us_period) {
    us_timestamp += us_period;
    if (_inputMode == inputMode::input || _inputMode == inputMode::input_pullup || _inputMode == inputMode::input_pulldown) {
      if (_inA) dat = digitalRead(_inA);
      if (_inB) dat += digitalRead(_inB) * 2;
    }
    if (_inputMode == inputMode::input_byte) dat = *_in;
    if (getInputInvert()) dat = ~dat;
    if (bit > 7) bit = 0;
    debouncer(bit);
  } // run sample
} // poll

void Toggle::setInputMode(inputMode inputMode) {
  _inputMode = inputMode;
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
  if (csr & 0b10000000) { // onPress
    csr &=  ~0b10000000; // clear onPress flag
    pOut = out;
    return true;
  }
  return false;
}

bool Toggle::onRelease() {
  if (csr & 0b01000000) { // onRelease
    csr &=  ~0b01000000; // clear onRelease flag
    pOut = out;
    return true;
  }
  return false;
}

uint8_t Toggle::onChange() {
  if (csr & 0b01000000) return 2; // onRelease
  else if (csr & 0b10000000) return 1; // onPress
  return 0;
}

bool Toggle::toggle() {
  if (!getToggleTrigger()) {
    if (((!getInputInvert() && isPressed()) || (getInputInvert() && isReleased())) && !getLastToggleState()) {
      setLastToggleState(true);
      (getToggleState()) ? setToggleState(false) : setToggleState(true); // toggle
    }
    if (((!getInputInvert() && isReleased()) || (getInputInvert() && isPressed())) && getLastToggleState()) setLastToggleState(false);
    return getToggleState();
  } else {
    if (((!getInputInvert() && isReleased()) || (getInputInvert() && isPressed())) && !getLastToggleState()) {
      setLastToggleState(true);
      (getToggleState()) ? setToggleState(false) : setToggleState(true); // toggle
    }
    if (((!getInputInvert() && isPressed()) || (getInputInvert() && isReleased())) && getLastToggleState()) setLastToggleState(false);
    return !getToggleState();
  }
}

/********* button status register functions **********/

void Toggle::setInputInvert(bool invert) {
  (invert) ? csr |= 0b00000010 : csr &= ~0b00000010;
}

bool Toggle::getInputInvert() { // private
  return csr & 0b00000010;
}

void Toggle::setToggleTrigger(bool change) {
  (change) ? csr |= 0b00000100 : csr &= ~0b00000100;
}

bool Toggle::getToggleTrigger() { // private
  return csr & 0b00000100;
}

void Toggle::setToggleState(bool toggleState) {
  (toggleState) ? csr |= 0b00010000 : csr &= ~0b00010000;
}

bool Toggle::getToggleState() { // private
  return csr & 0b00010000;
}

void Toggle::setLastToggleState(bool lastToggleState) {
  (lastToggleState) ? csr |= 0b00100000 : csr &= ~0b00100000;
}

bool Toggle::getLastToggleState() { // private
  return csr & 0b00100000;
}

/************* button timer functions ****************/

void Toggle::clearTimer() {
  startUs = micros();
}

uint32_t Toggle::getElapsedMs() {
  return (micros() - startUs) * 0.001;
}

bool Toggle::blink(uint16_t ms, uint8_t mode) {
  if (mode == 2 && onChange() == 2) clearTimer();
  else if (mode == 1 && onChange() == 1) clearTimer();
  else if (mode == 0 && onChange()) clearTimer();
  onPress();
  onRelease();
  return (bool)(ms > (getElapsedMs()));
}

bool Toggle::pressedFor(uint16_t ms) {
  if (isPressed() && getElapsedMs() > ms) {
    return true;
  }
  return false;
}

bool Toggle::releasedFor(uint16_t ms) {
  if (isReleased() && getElapsedMs() > ms) {
    return true;
  }
  return false;
}

bool Toggle::retrigger(uint16_t ms) {
  if (isPressed() && getElapsedMs() > ms) {
    clearTimer();
    return true;
  }
  return false;
}

uint8_t Toggle::pressCode(bool debug) {
  static uint8_t pCode = 0, code = 0;
  static uint32_t elapsedMs = 0;

  switch (_state) {
    case PB_DEFAULT:
      elapsedMs = getElapsedMs();
      if (pCode && isReleased() && (elapsedMs > (CLICK::LONG + CLICK::MULTI))) _state = PB_DONE;
      if (onChange()) clearTimer();
      if (onPress()) {
        _state = PB_ON_PRESS;
      }
      if (onRelease()) {
        if (debug) {
          Serial.print(F(" Pressed for:\t")); Serial.print(elapsedMs); Serial.println(" ms");
        }
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
      if (debug) {
        Serial.print(F(" Code:\t\t")); Serial.println(pCode, HEX); Serial.println();
      }
      code = pCode;
      pCode = 0;
      _state = PB_DEFAULT;
      if (code) return code;
      break;

    default:
      _state = PB_DEFAULT;
      break;
  }
  return 0;
}

/************** debouncer ************************************************************************
  The debouncer() function by default uses a robust algorithm that ignores up to 2 spurious
  signal transitions (glitches) and only adds up to 2 sample periods time lag to the output signal.
**************************************************************************************************/
uint8_t Toggle::debouncer(uint8_t bit) {
  pOut = out;
  uint8_t bits = 2;
  if (_inputMode == inputMode::input_byte) bits = 8;

  for (int i = bit; i < bit + bits; i++) {
    if ((dat & (1 << i)) && (pDat & (1 << i)) && (ppDat & (1 << i))) out |= (1 << i);
    else if (!(dat & (1 << i)) && !(pDat & (1 << i)) && !(ppDat & (1 << i))) out &= ~(1 << i);
  }
  if ((pOut & (1 << bit)) && !isReleased(bit)) {
    csr |=  0b10000000; // set onPress
    csr &= ~0b01000000; // clear onRelease
  } else {
    if ((!(pOut & (1 << bit))) && isReleased(bit)) {
      csr |=  0b01000000; // set onRelease
      csr &= ~0b10000000; // clear onPress
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
