#include "Arduino.h"
#include "Potenciometro.h"
#include "SoftTimer.h"

Potenciometro::Potenciometro(int pin, void (*onHandler)(int value)): Task(POLLING_TIME_MS, &(Potenciometro::step)) {

  this->_pin = pin;
  this->_onHandler = onHandler;
}


void Potenciometro::begin() {

  this->_idle = 0;
  this->_idle = this->_readPotenciometro();

  SoftTimer.add(this);
}

int Potenciometro::_readPotenciometro() {
  return (analogRead(this->_pin) >> 1) - this->_idle;
}

int Potenciometro::getValue() {
  return this->_last;
}

void Potenciometro::setHandler(void (*onHandler)(int value)) {
  this->_onHandler = onHandler;
}

void Potenciometro::step(Task* task) {
  Potenciometro* me = (Potenciometro*)task;
  int v = me->_readPotenciometro();

  if (v != me->_last) {
    me->_last = v;
    if (me->_onHandler != NULL) me->_onHandler(v);
  }

}
