#pragma once
#ifndef Toggle_h
#define Toggle_h

class Toggle {

  public:

    enum class sw : uint8_t {x, isUp, isMid, isDn, upToMid, midToDn, dnToMid, midToUp};
    Toggle(uint8_t pinA, uint8_t pinB, bool polarity);
    Toggle(uint8_t pinA, uint8_t pinB);
    Toggle(uint8_t pinA);
    uint8_t poll();
    bool isUp();
    bool isMid();
    bool isDn();
    bool wasUp();
    bool wasMid();
    bool wasDn();
    bool upToMid();
    bool midToDn();
    bool dnToMid();
    bool midToUp();

  private:

    uint8_t _pinA, _pinB, pinB = 0xFF, swStatus = 0, lastSwStatus = 0;
    bool _polarity, polarity, firstRun = true;
    sw sw = sw::x;
};
#endif
