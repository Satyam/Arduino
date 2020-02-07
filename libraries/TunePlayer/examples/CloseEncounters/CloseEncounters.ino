#include <TunePlayer.h>

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
  TunePlayer.onEnd(&onEndHandler);

  // Play it slow
  TunePlayer.setNoteDuration(400);
  TunePlayer.setGap(30);

  // Play it!
  TunePlayer.play(tune);

}
