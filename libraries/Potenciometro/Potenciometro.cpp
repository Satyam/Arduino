#include "Arduino.h"
#include "Potenciometro.h"
#include "SoftTimer.h"

Potenciometro::Potenciometro(int pin, void (*onHandler)(int value)): MyTask(50) {

  this->_pin = pin;
  this->_onHandler = onHandler;
}



void Potenciometro::init() {
  this->_idle = 0;
  this->_idle = this->_readPotenciometro();
  Task::init();
}


int Potenciometro::_readPotenciometro() {
  return (analogRead(this->_pin) >> 1) - this->_idle;
}

int Potenciometro::getValue() {
  if (this->isActive()) {
    return this->_last;
  }
  return this->_readPotenciometro();
}

void Potenciometro::setHandler(void (*onHandler)(int value)) {
  this->_onHandler = onHandler;
}


void Potenciometro::step() {
  int v = this->_readPotenciometro();

  if (v != this->_last) {
    this->_last = v;
    if (this->_onHandler != NULL) this->_onHandler(v);
  }
}
