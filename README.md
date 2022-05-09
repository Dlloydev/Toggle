



# Toggle    [![arduino-library-badge](https://www.ardu-badge.com/badge/Toggle.svg?)](https://www.ardu-badge.com/Toggle) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/dlloydev/library/Toggle.svg)](https://registry.platformio.org/libraries/dlloydev/Toggle)

Arduino library for deglitching and debouncing signals, buttons and switches.  One or two inputs can be monitored to decode transistions (depicting direction) for SPST, SPDT or SP3T contacts. Simple to use and requires very little memory. Captures one-shot transitions (depicting direction) and current status. 

![image](https://user-images.githubusercontent.com/63488701/167322856-91eba08a-687d-4b0b-813a-97d32b092327.png)

#### Features

- Performs both debouncing and deglitching.
- Works with signals, stored data, buttons and switches.
- External pull-up resistors not required.
- Very simple to use.
- Ultra low memory use.

## Using Toggle

Simple to use because pinMode, input pullups, de-glitch period, bounce period, and switch type (1 or 2 inputs) is automatically detected and configured.

Declaring a switch or button using 1 digital pin for SPST or SPDT contacts:

```c++
Toggle sw1(6); // GPIO 6
```

Declaring a switch or button using 2 digital pins for SP3T contacts:

```c++
Toggle sw2(7, 8); // GPIO 7 and 8
```

Each switch is polled  in the `loop()` function:

```c++
sw1.poll();
sw2.poll();
```

#### The switch functions when using 1 input pin:

```c++
bool isOFF();
bool isON();
bool OFFtoON();
bool ONtoOFF();
```

The switch has 2 positions referred to as OFF (input is high) and ON (input is low). The first 2 functions will continuously return true if the switch is at that current position. The last 2 functions return true (once only) if the switch has just transitioned to the checked position. This is very handy to execute code based on direction of switched operation or for any one-shot processing of code.

#### The switch functions when using 2 input pins:

```c++
bool isUP();
bool isMID();
bool isDN();
bool UPtoMID();
bool MIDtoDN();
bool DNtoMID();
bool MIDtoUP();
```

The switch has 3 positions referred to as UP, MID (center) and DN (down). The first 3 functions will continuously return true if the switch is at that position. The last 4 functions return true (once only) if the switch has just transitioned to that position. This is very handy to execute code based on direction of switched operation or for any one-shot processing of code.

#### Other functions:

```c++
void setInputMode(inMode inputMode);

// options:
sw1.setInputMode(sw1.inMode::input_input);     // high impedance input
sw1.setInputMode(sw1.inMode::input_pullup);    // pullup resistor enabled (default)
sw1.setInputMode(sw1.inMode::input_pulldown);  // not used 
sw1.setInputMode(sw1.inMode::input_logic);     // uses a byte variable for input data

void setSampleUs(uint16_t sampleUs);

// default sample period is 5000μs, range is 0-65535μs (0-65ms)
// using 8 samples, the range for debounce will be (0-524ms)
sw1.setSampleUs(5000); //μs
```

#### Example Sketch:

This sketch checks the status of a SP3T and a basic SPST switch or button:

```c++
#include <Toggle.h>

Toggle sw1(6,7);
Toggle sw2(8);

void setup() {
  while (!Serial) { }; // Leonardo
  Serial.begin(115200);
}

void loop() {
  sw1.poll();
  sw2.poll();

  if (sw1.UPtoMID) Serial.println(F("sw1: UP⇒MID"));
  if (sw1.MIDtoDN) Serial.println(F("sw1: MID⇒DN"));
  if (sw1.DNtoMID) Serial.println(F("sw1: DN⇒MID"));
  if (sw1.MIDtoUP) Serial.println(F("sw1: MID⇒UP"));

  if (sw2.OFFtoON) Serial.println(F("sw2: OFF⇒ON"));
  if (sw2.ONtoOFF) Serial.println(F("sw2: ON⇒OFF"));
}
```

## Switch Connections

Switching between GND and digital pin is the default solution used by this library. External pull-up resistors can be added if desired, but the internal pullups should be sufficient for most applications. What might be of consideration is providing sufficient wetting current to overcome switch contact oxidation.

A set of connections are shown where 0.1μF capacitors are optionally added to provide the following benefits:

- contact wetting current
- hardware signal filtering 
- beneficial for interrupt applications
- improves noise immunity when using longer cables 

#### Connections shown below are for 2-position switches (SPST, SPDT, DPDT) using 1 input:

![image](https://user-images.githubusercontent.com/63488701/166920176-7bd21bb6-10f9-4cd1-9467-0c2289e698c5.png)

![image](https://user-images.githubusercontent.com/63488701/166920355-3edac199-4aae-4615-a790-152c2f3acec5.png)

 

#### Connections shown below are for 3-position switches (SP3T, DP3T) using 2 inputs:



#### Toggle Switch (SP3T, On-Off-On):

![image](https://user-images.githubusercontent.com/63488701/166512833-eda91d35-60bd-4846-95cb-326a442edfac.png)



#### Slide Switch (Single Pole, 3-position):

![image](https://user-images.githubusercontent.com/63488701/166516607-82f0c9c1-e627-4769-bb15-5df6c0bd8784.png)



#### Rotary Switch (Double Pole, 3-position):

![image](https://user-images.githubusercontent.com/63488701/166517355-0869726d-dca0-4125-bb3e-2bebe63f6afb.png)



#### Rocker Switch (SP3T, On-Off-On):

![image](https://user-images.githubusercontent.com/63488701/166518133-6c991e99-1618-404b-b9b4-62c0c0f79dae.png)



#### Polling

The switch inputs are polled in the main loop and the history of readings is stored in an 8-bit shift register (byte) where bit0 is the present reading and bit7 is the oldest reading. Data is shifted and updated every 5ms.

#### Deglitching

Using the input pullups provides a high 20K-50K impedance that makes the signals more susceptible to noise pickup, especially if longer cables are used in a noisy environment. To improve this possible situation, software deglitching is internally set to 1 read sample which represents a minimum period of 5ms that any reading change is ignored.

#### Debouncing

Debouncing requires the shift register to be completely filled with 1's or 0's to signify a stable state. This occurs 40ms after bouncing stops. Contact closure will be detected after 2 stable samples (readings) are made. This allows single sample anomalies to be ignored (deglitched). Contact release is detected when 8 stable samples (readings) have been made.

#### Memory Comparison on Leonardo with 2 buttons attached :

| Library      | Version   | Bytes   | Bytes Used |
| ------------ | --------- | ------- | ---------- |
| Empty sketch | --        | 149     | --         |
| JC_Button.h  | 2.1.2     | 186     | 37         |
| **Toggle.h** | **2.2.0** | **190** | **41**     |
| Bounce2.h    | 2.71.0    | 193     | 44         |
| AceButton.h  | 1.9.2     | 205     | 56         |
| ezButton.h   | 1.0.3     | 331     | 182        |

### References

- [A Guide To Debouncing](http://www.ganssle.com/item/debouncing-switches-contacts-code.htm)
- [Wetting Current](https://en.wikipedia.org/wiki/Wetting_current)
