# TunePlayer

Plays a tune in an attached passive buzzer.

Requires [SoftTimer](https://github.com/prampec/arduino-softtimer) to be installed.
Suggested: `pitches.h` installed by default with the [ToneMelody tutorial](https://www.arduino.cc/en/Tutorial/ToneMelody)

## Description

Creates a `TunePlayer` static object which allows simple tunes to be played.  
Since the `tone()` built-in function only allows for one tone to be played at any time, 
this object is a singleton, i.e.: only one can exist at any one time.

## Usage

Include the TunePlayer.h header file:

```C++
#include "TunePlayer.h"
```

### Define the tune

One or more tunes can be played.  They are defined as an array of unsigned integers 
containing tuples with frequencies for the notes 
(usually taken from `pitches.h` header file) and duration in default note lengths.
The end of the tune must be signalled with a `0` which is, otherwise, an invalid note.

```c++
unsigned int tune[] = {
  NOTE_AS4, 1,
  NOTE_C5,  1,
  NOTE_GS4, 1,
  NOTE_GS3, 1,
  NOTE_DS4, 3,
  0
};
```

The above corresponds to the tune in Close Encounters of the Third Kind.  Each of the first 4 notes are of equal duration, 
the last one is triple the duration of the others.

The arrangement of one tuple per line is for ease of reading, C++ doesn't mind.  Notice the 0 to signal the end!!.

### Attach the buzzer

Within the `setup()` function, attach the `TunePlayer` to the pin where the buzzer is connected:

```C++
// Elsewhere:
// #define BUZZER 8

TunePlayer.attach(BUZZER);
```

### Play the tune

At any point after attaching the buzzer in `setup()` a tune can be played. 
If a tune is already being played, no new tune can be played.
The `play()` method will return `true` if the call is successful,
`false` if it is already playing a tune. 

```c++
TunePlayer.play(tune);
```

### Check if available

The method `isActive` (inherited from `MyTask`) will return `true` while a tune is being played.  
No further tunes can be played while `isActive` is `true`.

```c++
if (TunePlayer.isActive()) {
  // tune is being played
}
```

### Stop playing

The inherited method `stop` will stop playing the current tune.

```c++
// Ensure no tune is being played or stop playing the current one
TunePlayer.stop();
// A new tune may be played 
```

### Adjusting play speed

The speed can be adjusted by setting the duration of a note in milliseconds and the gap between notes.  
By default it is set to 120bpm (beats per minute) with 4 notes per bar (or measure), with a 90% *sound on* time
and a 10% gap *sound off*.

This results in: 

 *  (60 minutes) * (1000 ms per second) / (120bpm) / (4 notes per bar) => (125 ms per note)
 *  Of those 125, we assigned 113 to sound on and 12 to the gap in between notes

 This can be changed via the methods:

 * `setNoteDuration`: in milliseconds, defaults to 113ms.
 * `setGap`: in milliseconds, defaults to 12ms.

 ```c++
 TunePLayer.setNoteDuration(300);
 TunePlayer.setGap(30);
 ```

 ### Event: `onEnd`

 To know when a tune has finished, an `onEnd` handler function can be set. It can be declared like this:

 ```c++
 void onEndHandler() {
  Serial.println("on end handler called");
}
```

The handler can be named anything, doesn't receive any argument nor does it need to return anything. 

To tell `TunePlayer` to call the handler, pass a reference to the `onEnd()` method:

```c++
TunePlayer.onEnd(onEndHandler);
```

## Example:

```c++
#include "TunePlayer.h"

// pin for the buzzer
#define BUZZER 8

// Tune to play
unsigned int tune[] = {
  NOTE_AS4, 1,
  NOTE_C5,  1,
  NOTE_GS4, 1,
  NOTE_GS3, 1,
  NOTE_DS4, 3,
  0  //         Important !!
};

// Handler function when the tune is finished
void onEndHandler() {
  Serial.println("on end handler called");
}

void setup() {
  Serial.begin(9600);

  // Attach the pin for the buzzer
  TunePlayer.attach(BUZZER);
  
  // Tell what to call when finished
  TunePlayer.onEnd(onEndHandler);

  // Play it slow
  TunePlayer.setNoteDuration(400);
  TunePlayer.setGap(30);

  // Play it!
  TunePlayer.play(tune);

}
```