# Toggle    [![arduino-library-badge](https://www.ardu-badge.com/badge/Toggle.svg?)](https://www.ardu-badge.com/Toggle) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/dlloydev/library/Toggle.svg)](https://registry.platformio.org/libraries/dlloydev/Toggle)

Arduino button library for debouncing switch contacts and input data.  New pressCode() function detects fast, short and long button presses for 225 combinations. Works with all switch types, port expanders and other 8-bit data sources. Debounce algorithm ignores several consecutive  spurious transitions.

## Features

### Flexible Inputs

The inputs can be from a single pin or several pins allowing the use of 2 or 3-position switches and up to seven debounced states. When linking to a data (byte) input, the debouncer can work with any selected bit or it can debounce all 8-bits in one Toggle instance. See [examples](https://github.com/Dlloydev/Toggle/tree/main/examples).

### Debounce Algorithm

The debounce algorithm adds only 2 sample periods of time lag to the output signal. A 3-sample stable period is required for an output bit to change. Therefore, to set an output bit, 3 consecutive 1's are required. When 3 consecutive 0's are detected, that bit value is cleared.![image](https://user-images.githubusercontent.com/63488701/171260623-befe88a4-66c4-44a2-a38b-6c14c715a92d.png)

| ppDat | pDat | dat  | **R** | **S** | **Q**      | **State** |
| ----- | ---- | ---- | ----- | ----- | ---------- | --------- |
| 0     | 0    | 0    | 1     | 0     | 0          | Reset     |
| 0     | 0    | 1    | 0     | 0     | Last State | No Change |
| 0     | 1    | 0    | 0     | 0     | Last State | No Change |
| 0     | 1    | 1    | 0     | 0     | Last State | No Change |
| 1     | 0    | 0    | 0     | 0     | Last State | No Change |
| 1     | 0    | 1    | 0     | 0     | Last State | No Change |
| 1     | 1    | 0    | 0     | 0     | Last State | No Change |
| 1     | 1    | 1    | 0     | 1     | 1          | Set       |

### Sampling

The sample period defaults to 5000 μs. With this setting, only 15ms is required for detecting a button switch being pressed or released. This may seem low when thinking of regular debouncig, but in order for this method to falsely detect a transition, it would require that there be a gap of greater than 15ms between bounces. From *[A Guide to Debouncing](http://www.ganssle.com/item/debouncing-switches-contacts-code.htm)*, (Anatomy of a Bounce):

> *Consider switch E again, that one with the pretty face that hides a  vicious 157 msec bouncing heart. One test showed the switch going to a  solid one for 81 msec, after which it dropped to a perfect zero for 42  msec before finally assuming its correct high state. Think what that  would do to pretty much any debounce code!* 

Using the Toggle library, this switch could be debounced with a 15ms sample period and ignoring dropouts of up to 45ms.

## Switch Connections

Switching between GND and digital pin is the default solution used by this library. External pull-up resistors can be added if desired, but the internal pullups should be sufficient for most applications. What might be of consideration is providing sufficient wetting current to overcome switch contact oxidation.

A set of connections are shown where 0.1μF capacitors are optionally added. This adds contact wetting current and hardware signal filtering (beneficial for interrupt applications) . 



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

#### The switch functions when using 2 input pins:

```
isUP();
isMID();
isDN();
UPtoMID();
MIDtoDN();
DNtoMID();
MIDtoUP();
```

The switch has 3 positions referred to as UP, MID (center) and DN (down). The first 3 functions will continuously return true if the switch is at that position. The last 4 functions return true (once only) if the switch has just transitioned to that position.



## Toggle Library Reference

To use this library

```c++
#include <Toggle.h>
```



## Constructors

```c++
Toggle();
Toggle(inA);
Toggle(inA, inB);
Toggle(*in);
```

##### Description

The constructor defines a button object. If the default constructor is used when declaring an array of pointers to button objects,        then it must be followed by a call to begin in setup.

##### Required parameter

**inA:**  Arduino pin number that the button or switch is connected to *(byte)* , <u>or</u>
***in:**  Arduino variable representing the input signal *(byte)*

##### Optional parameter

**inB:**  Second Arduino pin number *(byte)*. Use 2 inputs when connecting to 3 position switches.

##### Returns

None.

##### Example

```c++
Toggle myInput(2);      // Button connected pin 2 to GND, INPUT_PULLUP, debounced
Toggle myInput(2, 3);   // SPDT switch connected pins 2 and 3 to GND, INPUT_PULLUP, debounced
Toggle myInput(*Input); // Byte variable as input, debounced
```



## poll()

##### Description

This function is placed at the top of the loop. Initializes the Button object and the pin it is connected to.

##### Syntax

`myInput.poll();`

##### Optional parameter

**bit:** selects the bit number from an input data *(byte)*

##### Returns

None.



## Primary Functions

There are 5 primary functions when using 1 input pin or data bit. Shown below is a plot showing the returned values that are verically offset for clarity:

![image](https://user-images.githubusercontent.com/63488701/169307791-e4b02e03-4399-4984-87a2-755dafaf3f47.png)



## onChange()

##### Description

This function checks the status register to see if the button or switch has changed state and reports whether the onPress or onRelease flag has been set. Calling onChange() does not modify the status register.

##### Syntax

`myInput.onChange();`

##### Parameters

None.

##### Returns

no change (0) , onPress (1), onRelease (2) *(byte)*

##### Example

```c++
if (myInput.onChange() == 2) {
	// button was released
} else if (myInput.onChange() == 1) {
  // button was pressed
} else {
  // no change
}
```



## onPress()

## onRelease()

##### Description

These functions check the the status register to see if the button or switch has set the onPress or onRelease flag. These functions will return the flag status and clear the respective flag.

##### Syntax

`myInput.onPress();`  

`myInput.onRelease();`

##### Parameters

None.

##### Returns

*true* or *false*, *(bool)*

##### Example 1

```c++
if (myInput.onPress())
{
	// do something (true only once per press)
}
```

 

## isPressed()

## isReleased()

##### Description

These functions checks the curent debounced output and its history to see if the button or switch is pressed or released.

##### Syntax

`myInput.isPressed();`  

`myInput.isReleased();`

##### Optional parameter

**bit:** selects the bit number from an input data *(byte)*

##### Returns

*true* or *false*, *(bool)*

##### Example

```c++
if (myButton.isPressed()) {
	// do something
}
else {
	// do something else
}
```



## toggle()

##### Description

This function can be used to convert a momentary push button to a toggle switch. By default, the retuen value will toggle `true-false` then `false-true` for  each `onPress` action of the button.

- The `setToggleState()` function sets the initial state (*true* or *false*)
- The `setToggleTrigger()` function sets the trigger mode: *false*: `onPress`, *true*: `onRelease`

##### Parameters

None.

##### Returns

*true* or *false*, (*bool*). Toggles as configured by `setToggleTrigger()`. Default is trigger `onPress`



## Timer Functions

## clearTimer()

##### Description

Simply clears the ms timer used for the timer functions. 

##### Syntax

`myInput.clearTimer();` 

##### Parameters

None.

##### Returns

None.



## blink(ms, mode)

##### Description

This function sets the duration in milliseconds that the returned value is true. The mode parameter sets what blink responds to: onChange (0), onPress( 1) default, onRelease (2).

##### Syntax

`myInput.blink(ms);` 

##### Parameters

**ms:** The number of milliseconds *(unsigned int)*

**mode:** Blink  onChange (0), onPress( 1) default, onRelease (2) *(byte)*

##### Returns

*true* or *false*, depending on whether the elapsed time has expired after any state change set by the mode parameter. *(bool)*

##### Example

 [Toggle_Basic.ino](https://github.com/Dlloydev/Toggle/blob/main/examples/Toggle_Basic/Toggle_Basic.ino)



## pressedFor(ms)

## releasedFor(ms)

##### Description

These functions return true if the button or switch has been in the pressedFor or releasedFor state for at least the given number of milliseconds.

##### Syntax

`myInput.pressedFor(ms);`  
`myInput.releasedFor(ms);`

##### Parameters

**ms:** The number of milliseconds *(unsigned int)*

##### Returns

*true* or *false*, depending on whether the elapsed time has expired after the state change. *(bool)*

##### Example

```c++
if (myInput.pressedFor(500)) {
   // true (once only) if button has been pressed for 500ms
}
```

 

## retrigger(ms)

##### Description

This function checks the duration in milliseconds that the  button or switch is is in the state as selected by timer mode and returns true (once only) each time the given millisecond duration has expired.

##### Syntax

`myInput.retrigger(ms);` 

##### Parameters

**ms:** The number of milliseconds *(unsigned int)*

##### Returns

*true* or *false*, returns true (once only) each time the given ms duration has expired while the button is in the state as selected by timer mode. *(bool)*

##### Example

```c++
if (retrigger(500)) {
// count every 500ms interval while the button is being pressed   
} 
```

 

## pressCode()

##### Description

- Up to 225 possible codes with one button. The returned code (*byte*) is easy to interpret when viewed in hex format. For example, `47` is 4 long, 7 short presses. `F2` is double-click, `F7` is 7 `F`ast clicks. 
- Fast-click mode is detected if the first 2 clicks (presses) are less than 0.4 sec, then counts any extra presses if the duration is less than 1 sec, up to 15 max (code `FF`) 
- Detection of long (greater than 1 sec) presses and short (less than 1 sec) presses occurs if the first press is 0.4 sec or longer. 
- Detect up to 15 short presses
- Detect up to 14 long presses
- Returns code after button is released for 1.4 sec
- simplifies your code while adding maximum functionality to one button

##### Example

```c++
byte pCode = sw1.pressCode(1); // (1) serial print results
```

##### Example Sketch

[Press_Code.ino](https://github.com/Dlloydev/Toggle/blob/main/examples/Press_Code/Press_Code.ino)



## Set and Get Functions



## setInputMode()

##### Description

This function sets the various options for the input source. 

##### Syntax

`myInput.inMode::input_option;` 

##### Returns

None.

##### Options

```c++
myInput.setInputMode(sw1.inMode::input_input);     // high impedance input
myInput.setInputMode(sw1.inMode::input_pullup);    // pullup resistor enabled (default)
myInput.setInputMode(sw1.inMode::input_pulldown);  // pulldown resistor enabled (ESP32) 
myInput.setInputMode(sw1.inMode::input_byte);      // input byte (8-bit)
```

 

## setInputInvert()

##### Description

Set true if the button or switch pulls the signal high when pressed. Default is false (button or switch pulls the signal low when pressed).

##### Syntax

`myInput.setInputInvert(true);` 

##### Returns

None.



## setSampleUs()

##### Description

Sets the sample period in microseconds. Default is 5000 μs.

##### Syntax

`myInput.setSampleUs(us);` 

##### Returns

None.



## getElapsedMs(()

##### Description

Gets the elapsed ms since the last state change selected by timer mode.

##### Syntax

`myInput.getElapsedMs();` 

##### Returns

Elapsed milliseconds *(unsigned int)*.



---



### References

- [A Guide To Debouncing](http://www.ganssle.com/item/debouncing-switches-contacts-code.htm)
- [Wetting Current](https://en.wikipedia.org/wiki/Wetting_current)

