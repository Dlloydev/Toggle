# Toggle    [![arduino-library-badge](https://www.ardu-badge.com/badge/Toggle.svg?)](https://www.ardu-badge.com/Toggle) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/dlloydev/library/Toggle.svg)](https://registry.platformio.org/libraries/dlloydev/Toggle)

Arduino button debounce library for various switch types, port expanders and other 8-bit data sources. Fast and robust debounce algorithm.

## Features

- Fast, robust and symetrical [debouncing](https://github.com/Dlloydev/Toggle/wiki/Debounce-Algorithm) of both press and release button transitions
- Works with various [switch types and connections](https://github.com/Dlloydev/Toggle/wiki/Switch-Connections).
- Identifies 7 unique transitions for 3-position switches
- Use momentary button as toggle switch (configurable)
- Return up to 225 codes from one button

## Examples

- Lets give some examples a [trial run](https://github.com/Dlloydev/Toggle/wiki/Wokwi-Examples).

- See sketch [examples](https://github.com/Dlloydev/Toggle/tree/main/examples).



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

##### Example Sketch  [Press_Code.ino](https://github.com/Dlloydev/Toggle/blob/main/examples/Press_Code/Press_Code.ino)

[Trial run on *WOKWi*](https://wokwi.com/projects/334284248581145170)



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

