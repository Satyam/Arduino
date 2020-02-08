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

  joystickX.start();
  joystickY.start();

  // ......
}
```

On starting, it will read the current value for the potentiometer and use that value as the center or idle reference value.

### Stop/suspend polling

Polling of the potentiometer can be stopped at any time and later resumed.  The status can be read at any time:

```c++
Serial.println(joystickX.isActive());  // will print true
joystickX.stop();
Serial.println(joystickX.isActive());  // will print false

// and at a later time
joystickX.start();
Serial.println(joystickX.isActive());  // will print true
```

Calling either `start` or `stop` more than once consecutively has does not hurt (meaning, you don't need to check whether it is active or not to change it).

### Setting polling interval.

The default polling interval is set at 50ms.  It can be changed with the `SetPollingInterval` function.

```c++
joystickX.setPollingInterval(100);
```

The value is in milliseconds and has a maximum of 4,294,967.

### Change pin and handler

Either can be changed by calling the following functions:

```c++
  void setPin(int pin);
  void setHandler(void (*onHandler)(int value));
```

### Read the value


```c++
  int getValue();
```

If polling is active, it will provide the latest value read.  If it is not polling it will actually read the current value.
