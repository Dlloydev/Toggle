# Toggle
##  About

Arduino switch and button library for components having 3 switched positions of control (SP3T or On-Off-On). Simple to use, automatically configures the pin mode and uses very little memory. Ten status functions available include one-shot transition status (depicting direction),  present status and previous position status. 

#### General features

- Performs both de-bouncing and de-glitching.
- External pull-up resistors not required.
- Very simple to use.
- Very low memory use.

## Using Toggle

Declare each switch or button, by indicating any 2 digital pins:

```c++
Toggle sw1(3, 4); // GPIO 3 and 4
```

The library sets pin in input mode with the internal  pull-up resistor enabled by default. All switches have to be repeatedly polled which is done in the `loop()` function.

```c++
sw1.poll();
```

The switch status can be checked with the following 10 get functions:

```c++
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
```

The switch has 3 positions referred to as Up, Mid (center) and Dn (down). The first 3 functions will continuously return true if the switch is at that current position. The next 3 functions will also continuously return true if the switch was previously at that position. The last 4 functions return true (once only) if the switch has just transitioned to the checked position. This is very handy to execute code based on direction of switched operation or for any one-shot processing of code.

#### Example Sketch

This sketch prints every sw1 transition (fully debounced and deglitched):

```c++
#include <Toggle.h>

Toggle sw1(3, 4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  sw1.poll();

  if      (sw1.upToMid()) Serial.println(F("Up⇒Mid"));
  else if (sw1.dnToMid()) Serial.println(F("Dn⇒Mid"));
  else if (sw1.midToUp()) Serial.println(F("Mid⇒Up"));
  else if (sw1.midToDn()) Serial.println(F("Mid⇒Dn"));
}
```

## Switch Connections

Switching between GND and digital pin is the default solution used by this library. External pull-up resistors can be added if desired, but the internal pullups should be sufficient for most applications. What might be of consideration is providing sufficient wetting current to overcome switch contact oxidation.

A set of connections are shown where 0.1μF capacitors are added to provide the following benefits:

- contact wetting current
- hardware signal filtering 
- beneficial for interrupt applications
- improves noise immunity when using longer cables 

Connections are shown both without and with capacitors installed.



#### Toggle Switch (SP3T, On-Off-On):

![image](https://user-images.githubusercontent.com/63488701/166512833-eda91d35-60bd-4846-95cb-326a442edfac.png)



#### Slide Switch (Single Pole, 3-position):

![image](https://user-images.githubusercontent.com/63488701/166516607-82f0c9c1-e627-4769-bb15-5df6c0bd8784.png)



#### Rotary Switch (Double Pole, 3-position):

![image](https://user-images.githubusercontent.com/63488701/166517355-0869726d-dca0-4125-bb3e-2bebe63f6afb.png)



#### Rocker Switch (SP3T, On-Off-On):

![image](https://user-images.githubusercontent.com/63488701/166518133-6c991e99-1618-404b-b9b4-62c0c0f79dae.png)



### Software Signal Conditioning

When monitoring the status of a 3-position switch, 2 inputs are used. Both software debouncing and deglitching are used for all 10 switch status conditions.

#### Polling

The switch inputs are polled in the main loop and the history of readings is stored in an 8-bit shift register (byte) where bit0 is the present reading and bit7 is the oldest reading. Data is shifted and updated every 10ms.

#### Deglitching

Using the input pullups provides a high 20K-50K impedance that makes the signals more susceptible to noise pickup, especially if longer cables are used in a noisy environment. To improve this possible situation, software deglitching is internally set to 1 read sample which represents a minimum period of 10ms that any reading change is ignored.

#### Debouncing

Debouncing requires the shift register to be completely filled with 1's or 0's to signify a stable state. This occurs 80ms after the last transition. For the 2 signals that are monitored, contact closure will be detected in after at least 10ms have elapsed (de-glitch period). Contact release is detected in at least 80ms. For the unconnected middle position of the switch, logic status is determined (not measured) in at least 80ms for both contact closure and release. 

### References

- [A Guide To Debouncing](http://www.ganssle.com/item/debouncing-switches-contacts-code.htm)
- [Wetting Current](https://en.wikipedia.org/wiki/Wetting_current)
