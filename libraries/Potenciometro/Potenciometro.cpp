#include "Arduino.h"
#include "Potenciometro.h"
#include "SoftTimer.h"

Potenciometro::Potenciometro(int pin, void (*onHandler)(int value)): Task(50, &(Potenciometro::step)) {

  this->_pin = pin;
  this->_onHandler = onHandler;
  this->_isActive = false;
}


void Potenciometro::start() {
  SoftTimer.add(this);
  this->_isActive = true;
}

void Potenciometro::init() {
  this->_idle = 0;
  this->_idle = this->_readPotenciometro();
  Task::init();
}

void Potenciometro::stop() {
  SoftTimer.remove(this);
  this->_isActive = false;
}

bool Potenciometro::isActive() {
  return this->_isActive;
}

int Potenciometro::_readPotenciometro() {
  return (analogRead(this->_pin) >> 1) - this->_idle;
}

int Potenciometro::getValue() {
  if (this->_isActive) {
    return this->_last;
  }
  return this->_readPotenciometro();
}

void Potenciometro::setHandler(void (*onHandler)(int value)) {
  this->_onHandler = onHandler;
}

void Potenciometro::setPollingInterval(unsigned long ms) {
  this->setPeriodMs(ms);
};

void Potenciometro::step(Task* task) {
  Potenciometro* me = (Potenciometro*)task;
  int v = me->_readPotenciometro();

  if (v != me->_last) {
    me->_last = v;
    if (me->_onHandler != NULL) me->_onHandler(v);
  }
}
