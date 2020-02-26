#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include "Arduino.h"
#include "MyTask.h"

class CurrentSensor : public MyTask {
  public:
    CurrentSensor(int pin, void (*onHandler)(float value));
    float getValue();
    void setPin(int pin);
    void setHandler(void (*onHandler)(float value));
    void setTotalCount(int totalCount);
    void setMaxCurrent(float amps);
    void setDiscardExtremes(bool discard);
    
    void init() override;
    void step() override;
    void setPollingInterval(unsigned long ms) override;


  private:
    int _pin;
    void (*_onHandler)(float value);
    float _factor;
    float _idle;
    float _sum;
    int _cnt;
    int _totalCount;
    bool _discardExtremes;
    float _minValue;
    float _maxValue;

};
#endif