# CurrentSensor

Handles a ACS712 current sensor via polling using [SoftTimer](https://github.com/prampec/arduino-softtimer).  

Requires [SoftTimer](https://github.com/prampec/arduino-softtimer) to be installed.

## Description

It will repeatedly poll an ACS712 Current Sensor or similar, to produce an average, which is less prone to transient noises than the raw reading.

By default it polls the sensor every 10 milliseconds and averages over 10 readings, thus producing an average every 100ms, approximately.  A handler function will be called with this value, in milliamperes, as its single argument.

The current sensor comes in ranges of 5A, 20A and 30A.  To scale the reading to mA, it needs to know the type of sensor used.  It defaults to the 5A.

On creation, it requires:

* `int pin`  The analog input pin where the signal is received, usually one of the `A0` to `A`*n* constants.
* A handler function to respond to changes in the input. The function will receive the value of the signal, approximate in mA with the default settings.

## Usage

Include the CurrentSensor.h header file:

```C++
#include "CurrentSensor.h"
```

### Define the callback function

The function will be called every 100ms with the default values of polling interval and sample size.

```c++
void onCs(float value) {
  Serial.print("value: ");
  Serial.println(value);
}
```

### Create the instance

On creation the pin the sensor is attached to, and the callback function are needed.  The callback function is not required and can be given as `NULL`.

```c++
CurrentSensor cs(A2, &onCs);
```

### Start sampling

Within the `setup()` function, start the sampling:

```c++

void setup() {
  Serial.begin(9600);

  cs.start();

  // ......
}
```

On starting, it assumes the consumer of the measured current is idle and it will use the first average as the idle reference value.

### Stop/suspend polling

Polling of the current sensor can be stopped at any time and later resumed.  The status can be read at any time:

```c++
Serial.println(cs.isActive());  // will print true
cs.stop();
Serial.println(cs.isActive());  // will print false

// and at a later time
cs.start();
Serial.println(cs.isActive());  // will print true
```

Calling either `start` or `stop` more than once consecutively does not hurt (meaning, you don't need to check whether it is active or not to change it).

### Setting the sensor range

The default is set for the 5A sensor.  To change the scale factor for other sensors, use the `setMaxCurrent` method.

```c++
  cs.setMaxCurrent(20);  // for the 20 ampere sensor
```

### Setting polling interval.

The default polling interval is set at 10ms.  It can be changed with the `SetPollingInterval` function.

```c++
cs.setPollingInterval(100);
```

The value is in milliseconds and has a maximum of 4,294,967.

### Setting the number of samples

The default number of samples per average is 10.  It can be changed with the `setTotalCount` method. The count should be a positive number, other values will have no effect.  A value of 1 will result in reading the raw value.  Too many samples may overflow the accumulator.

```c++
cs.setTotalCount(20);
```

### Change pin and handler

Either can be changed by calling the following functions:

```c++
  void setPin(int pin);
  void setHandler(void (*onHandler)(float value));
```

### Read the value


```c++
  float v = cs.getValue();
```

It will return the current value, in mA, read from the sensor.
