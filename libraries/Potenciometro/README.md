# Potenciometro

Handles a potentiometer via polling using [SoftTimer](https://github.com/prampec/arduino-softtimer).  

Requires [SoftTimer](https://github.com/prampec/arduino-softtimer) to be installed.

## Description

A potentiometer, or each of the two potentiometers in a joystick, can be read with this class.  
On creation, the class requires:

* `int pin`  The analog input pin where the signal is received, usually one of the `A0` to `A`*n* constants.
* A handler function to respond to changes in the input. The function will receive the value of the signal, in the -256 to 256 range.

## Usage

Include the Potenciometro.h header file:

```C++
#include "Potenciometro.h"
```

### Define the callback function

The function will be called if the value is different from that of the previous call. The raw analog value ranging from 0 to 1023 will be scaled to -255 to 255, which is the value passed to the function.

```c++
void onX(int value) {
  Serial.print("value: ");
  Serial.println(value);
}
```

### Create the instance

One or more, instances can be created, for example for the potentiometers on a joystick.
The callback function is not required and can be given as `NULL`.

```c++
Potenciometro joystickX(JOY_X, &onX);
Potenciometro joystickY(JOY_Y, &onY);
```

### Start reading

Within the `setup()` function, start the reading:

```c++

void setup() {
  Serial.begin(9600);

  joystickX.begin();
  joystickY.begin();

  // ......
}
```
### Change pin and handler

Either can be changed by calling the following functions:

```c++
  void setPin(int pin);
  void setHandler(void (*onHandler)(int value));
```

### Read the value

The latest value read from the potentiometer can be read at any time.

```c++
  int getValue();
```

