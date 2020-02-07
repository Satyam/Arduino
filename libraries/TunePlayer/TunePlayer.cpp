#include "TunePlayer.h"
#include <SoftTimer.h>

TunePlayerClass::TunePlayerClass(): Task(0, &(TunePlayerClass::_step))  {
  this->_noteDuration = DEFAULT_NOTE_DURATION;
  this->_gap = DEFAULT_GAP;
}

void TunePlayerClass::attach(unsigned int pin) {
  this->_pin = pin;
}

void TunePlayerClass::setNoteDuration( unsigned int noteDuration) {
  this->_noteDuration = noteDuration;
}

bool TunePlayerClass::play(unsigned int * tune) {
  if (this->isPlaying) return false;
  this->isPlaying = true;

  SoftTimer.remove(this);
  this->_tune = tune;

  this->setPeriodMs(0);
  SoftTimer.add(this);
  return true;
};

void TunePlayerClass::setGap(unsigned int gap) {
  this->_gap = gap;
}

void TunePlayerClass::onEnd(void (*onEndHandler)()) {
  this->_onEndHandler = onEndHandler;
}

void TunePlayerClass::_step(Task *task) {
  TunePlayerClass* me = (TunePlayerClass*)task;

  unsigned int note = *(me->_tune++);
  if (note == 0) {
    SoftTimer.remove(me);
    noTone(me->_pin);
    if (me->_onEndHandler)me->_onEndHandler();
    return;
  }
  int duration = me->_noteDuration * *(me->_tune++);
  tone(me->_pin, note, duration);
  task->setPeriodMs(duration + me->_gap);
}

TunePlayerClass TunePlayer;
