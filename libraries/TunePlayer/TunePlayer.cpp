#include "TunePlayer.h"
#include <SoftTimer.h>

TunePlayerClass::TunePlayerClass(): MyTask(0)  {
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
  if (this->isActive()) return false;
  this->stop();
  this->_tune = tune;

  this->setPollingInterval(0);
  this->start();
  return true;
};

void TunePlayerClass::setGap(unsigned int gap) {
  this->_gap = gap;
}

void TunePlayerClass::onEnd(void (*onEndHandler)()) {
  this->_onEndHandler = onEndHandler;
}

void stop() {
    MyTask::stop();
    noTone(this->_pin);
}

void TunePlayerClass::step() {

  unsigned int note = *(this->_tune++);
  if (note == 0) {
    this->stop();
    noTone(this->_pin);
    if (this->_onEndHandler)this->_onEndHandler();
    return;
  }
  int duration = this->_noteDuration * *(this->_tune++);
  tone(this->_pin, note, duration);
  this->setPollingInterval(duration + this->_gap);
}

TunePlayerClass TunePlayer;
