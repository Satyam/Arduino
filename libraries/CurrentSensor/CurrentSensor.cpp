#include "Arduino.h"
#include "CurrentSensor.h"
#include "SoftTimer.h"

CurrentSensor::CurrentSensor(int pin, void (*onHandler)(float value)): Task(10, &(CurrentSensor::step)) {
  _pin = pin;
  _onHandler = onHandler;
  _totalCount = 10;
}


void CurrentSensor::start() {
  if (!this->_isInitialized) {
    this->_cnt = 0;
    this->_sum = 0;
    this->_isInitialized = true;
    this->_idle = 0;
  }

  SoftTimer.add(this);
  this->_isActive = true;
}

void CurrentSensor::stop() {
  SoftTimer.remove(this);
  this->_isActive = false;
}

bool CurrentSensor::isActive() {
  return this->_isActive;
};

float CurrentSensor::getValue() {
  return (float(analogRead(this->_pin)) - this->_idle) * this.>_factor; 
};

void CurrentSensor::setMaxCurrent(float amps) {
  this->_factor = 512 / amps;
}

void CurrentSensor::setPin(int pin) {
  this->_pin = pin;
};

void CurrentSensor::setHandler(void (*onHandler)(float value)) {
  this->_onHandler = onHandler;
};

void CurrentSensor::setTotalCount(int totalCount) {
  if (totalCount > 0) this->_totalCount = totalCount;
};

void CurrentSensor::setPollingInterval(unsigned long ms) {
  this->setPeriodMs(ms);
};

void CurrentSensor::step(Task* task) {
  CurrentSensor* me = (CurrentSensor*)task;

  if (me->_cnt < me->_totalCount) {
    me->_sum += analogRead(me->_pin);
    me->_cnt++;
  } else {
    if (me->_idle) {
      if (me->_onHandler != NULL) me->_onHandler((float(me->_sum) / me->_totalCount - me->_idle) * me->_factor);
    } else {
      me->_idle = float(me->_sum) / me->_totalCount;
    }
    me->_cnt = 0;
    me->_sum = 0;
  }
}
