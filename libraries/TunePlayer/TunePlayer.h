#ifndef TUNE_PLAYER_CLASS_H
#define TUNE_PLAYER_CLASS_H

#include <Arduino.h>
#include <pitches.h>
#include <Task.h>

/* 
 * Based on 120bpm and 4 notes per beat, 
 *  60 minutes * 1000 ms per second / 120bpm / 4 notes per beat => 125ms per note
 *  Of those 125, we assigned 113 to sound on and 12 to the gap in between notes
 */
 
#define DEFAULT_NOTE_DURATION 113
#define DEFAULT_GAP 12

class TunePlayerClass: Task {
  public:
    TunePlayerClass();
    
    /**
     * Tell the player which pin has the buzzer attached
     */
    void attach(unsigned int pin);
    
    /**
     * Play the given tune
     */
    bool play(unsigned int *tune);
    
    /**
     * Set the duration in milliseconds for a note to be played
     */
    void setNoteDuration(unsigned int noteDuration);
    
    /**
     * Set the gap between one note and the next.  Can be 0 for *legato*
     */
    void setGap(unsigned int gap);
    
    /**
     * sets a reference to a function to be called whenver a tune ends.
     */
    void onEnd(void (*onEndHandler)());

    /**
     * true if a tune is being played.  
     */
    bool isPlaying;
    
  private:
    unsigned int _pin;
    unsigned int _noteDuration;
    unsigned int _gap;
    void (*_onEndHandler)();
    unsigned int* _tune;
    static void _step(Task *task);
};

/** 
 *  TunePlayer is a singleton of the above TunePlayerClass, since there can't be more than one tone played at once in Arduino.
 */
extern TunePlayerClass TunePlayer;

#endif
