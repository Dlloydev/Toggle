/****************************************************
   Toggle Library for Arduino - Version 1.0.0
   by dlloydev https://github.com/Dlloydev/Toggle
   Licensed under the MIT License.
 ****************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Toggle.h"

Toggle::Toggle(uint8_t pinA, uint8_t pinB, bool polarity)
{
  _pinA = pinA;
  _pinB = pinB;
  _polarity = polarity;
}

Toggle::Toggle(uint8_t pinA, uint8_t pinB) : Toggle::Toggle(pinA, pinB, polarity = HIGH) { }
Toggle::Toggle(uint8_t pinA) : Toggle::Toggle(pinA, pinB = 0xFF, polarity = HIGH) { }

uint8_t Toggle::poll() {
  if (firstRun) {
    firstRun = false;
    if (_polarity) {
      pinMode(_pinA, INPUT_PULLUP);
      pinMode(_pinB, INPUT_PULLUP);
    } else {
      pinMode(_pinA, INPUT);
      pinMode(_pinB, INPUT);
    }
  }
  static uint8_t prevA = 0xAA, prevB = 0x55;
  static uint32_t sampleTime, ms;

  if (ms - sampleTime > 10) {
    prevA = prevA << 1 | digitalRead(_pinA);
    prevB = prevB << 1 | digitalRead(_pinB);
    sampleTime = ms;
  }
  ms = millis();

  // set switch positon status
  if      (prevA == 0xFE && prevB == 0xFF) swStatus = static_cast<uint8_t>(sw::isUp);
  else if (prevB == 0xFE && prevA == 0xFF) swStatus = static_cast<uint8_t>(sw::isDn);
  else if (prevA == 0xFF && prevB == 0xFF) swStatus = static_cast<uint8_t>(sw::isMid);

  // set switch transition status
  if      (wasUp() && isMid()) swStatus = static_cast<uint8_t>(sw::upToMid);
  else if (wasMid() && isDn()) swStatus = static_cast<uint8_t>(sw::midToDn);
  else if (wasDn() && isMid()) swStatus = static_cast<uint8_t>(sw::dnToMid);
  else if (wasMid() && isUp()) swStatus = static_cast<uint8_t>(sw::midToUp);

  if (lastSwStatus != swStatus) sampleTime = ms;
  lastSwStatus = swStatus;
  return swStatus;
}

bool Toggle::isUp() {
  if (swStatus == static_cast<uint8_t>(sw::isUp)) return true;
  return false;
}

bool Toggle::isMid() {
  if (swStatus == static_cast<uint8_t>(sw::isMid)) return true;
  return false;
}

bool Toggle::isDn() {
  if (swStatus == static_cast<uint8_t>(sw::isDn)) return true;
  return false;
}

bool Toggle::upToMid() {
  if (swStatus == static_cast<uint8_t>(sw::upToMid)) return true;
  return false;
}

bool Toggle::midToDn() {
  if (swStatus == static_cast<uint8_t>(sw::midToDn)) return true;
  return false;
}

bool Toggle::dnToMid() {
  if (swStatus == static_cast<uint8_t>(sw::dnToMid)) return true;
  return false;
}

bool Toggle::midToUp() {
  if (swStatus == static_cast<uint8_t>(sw::midToUp)) return true;
  return false;
}

bool Toggle::wasUp() {
  if (lastSwStatus == static_cast<uint8_t>(sw::isUp)) return true;
  return false;
}

bool Toggle::wasMid() {
  if (lastSwStatus == static_cast<uint8_t>(sw::isMid)) return true;
  return false;
}

bool Toggle::wasDn() {
  if (lastSwStatus == static_cast<uint8_t>(sw::isDn)) return true;
  return false;
}
