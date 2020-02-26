#include "Arduino.h"
#include "CurrentSensor.h"
#include "SoftTimer.h"

CurrentSensor::CurrentSensor(int pin, void (*onHandler)(float value)): MyTask(10) {
  _pin = pin;
  _onHandler = onHandler;
  _totalCount = 10;
  _factor = 5000.0 / 512.0;
}



void CurrentSensor::init()
{
  this->_cnt = 0;
  this->_sum = 0.0;
  this->_idle = 0.0;
  this->_discardExtremes = false;
  MyTask::init();
}


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
  MyTask::setPollingInterval(ms);
  this->_cnt = 0;
  this->_sum = 0.0;
};

void CurrentSensor::setDiscardExtremes(bool discard) {
  this->_discardExtremes = discard;
}

void CurrentSensor::step() {
  bool discard = this->_discardExtremes;
  float value;

  if (this->_cnt < this->_totalCount) {
    value = float(analogRead(this->_pin));
    this->_minValue = min(this->_minValue, value);
    this->_maxValue = max(this->_maxValue, value);
    this->_sum += value;
    this->_cnt++;
  } else {
    value = discard
      ? (this->_sum  - this->_minValue - this->_maxValue) / float(this->_cnt - 2 )
      : this->_sum / float(this->_cnt);
    if (this->_idle > 0.0) {
      if (this->_onHandler != NULL) {
        this->_onHandler((value  - this->_idle)  * this->_factor);
      }
    } else {
      this->_idle = value;
    }
    this->_cnt = 0;
    this->_sum = 0.0;
    this->_minValue = 1023;
    this->_maxValue = 0;
  }
}
