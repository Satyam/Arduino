# Arduino

My Arduino projects, some sketches, other libraries.  Each has its own README.md file

## Projects

* [autito](autito) An imaginary car.  Handles a DC motor for propulsion, a servo for direction and a buzzer as a horn, via a Joystick
* [autito-int](autito-int) Same as above, but the button is handled via interrupts
* [autito-remote](autito-remote) Handles the same hardware both locally as per de above and also via commands sent through the USB connection and reports status to display in remote console

## Libraries

The following inherit from `Task` in [SoftTimer](https://github.com/prampec/arduino-softtimer)

* [TunePlayer](libraries/TunePlayer) Allows simple tunes to be played in a buzzer.
* [Potenciometro](libraries/Potenciometro) Simplifies reading of a potentiometer.
* [CurrentSensort](libraries/CurrentSensor) Allows polling and averaging of the ACS-712 current sensor.
* 

