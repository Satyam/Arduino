#include "Arduino.h"
#include "CurrentSensor.h"
#include "SoftTimer.h"

CurrentSensor::CurrentSensor(int pin, void (*onHandler)(float value)): Task(10, &(CurrentSensor::step)) {
  _pin = pin;
  _onHandler = onHandler;
  _totalCount = 10;
  _factor = 5000.0 / 512.0;
}


void CurrentSensor::start() {
  SoftTimer.add(this);
  this->_isActive = true;
}

void CurrentSensor::stop() {
  SoftTimer.remove(this);
  this->_isActive = false;
}

void CurrentSensor::init()
{
  this->_cnt = 0;
  this->_sum = 0.0;
  this->_idle = 0.0;
  this->_discardExtremes = false;
  Task::init();
}

bool CurrentSensor::isActive() {
  return this->_isActive;
};

float CurrentSensor::getValue() {
  return (float(analogRead(this->_pin))  - this->_idle ) * this->_factor; 
};

void CurrentSensor::setMaxCurrent(float amps) {
  this->_factor = amps * 1000.0 / 512.0;
}

void CurrentSensor::setPin(int pin) {
  this->_pin = pin;
};

void CurrentSensor::setHandler(void (*onHandler)(float value)) {
  this->_onHandler = onHandler;
};

void CurrentSensor::setTotalCount(int totalCount) {
  if (totalCount > 0) this->_totalCount = totalCount;
  this->_cnt = 0;
  this->_sum = 0.0;
};

void CurrentSensor::setPollingInterval(unsigned long ms) {
  this->setPeriodMs(ms);
  this->_cnt = 0;
  this->_sum = 0.0;
};

void CurrentSensor::setDiscardExtremes(bool discard) {
  this->_discardExtremes = discard;
}

void CurrentSensor::step(Task* task) {
  CurrentSensor* me = (CurrentSensor*)task;

  bool discard = me->_discardExtremes;
  float value;

  if (me->_cnt < me->_totalCount) {
    value = float(analogRead(me->_pin));
    me->_minValue = min(me->_minValue, value);
    me->_maxValue = max(me->_maxValue, value);
    me->_sum += value;
    me->_cnt++;
  } else {
    value = discard
      ? (me->_sum  - me->_minValue - me->_maxValue) / float(me->_cnt - 2 )
      : me->_sum / float(me->_cnt);
    if (me->_idle > 0.0) {
      if (me->_onHandler != NULL) {
        me->_onHandler((value  - me->_idle)  * me->_factor);
      }
    } else {
      me->_idle = value;
    }
    me->_cnt = 0;
    me->_sum = 0.0;
    me->_minValue = 1023;
    me->_maxValue = 0;
  }
}
