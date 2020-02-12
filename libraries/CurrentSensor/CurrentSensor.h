#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include "Arduino.h"
#include "Task.h"

class CurrentSensor : public Task {
  public:
    CurrentSensor(int pin, void (*onHandler)(float value));
    void start();
    void stop();
    bool isActive();
    float getValue();
    void setPin(int pin);
    void setHandler(void (*onHandler)(float value));
    void setTotalCount(int totalCount);
    void setPollingInterval(unsigned long ms);
    void setMaxCurrent(float amps);
    void setDiscardExtremes(bool discard);
    void init() override;

  private:
    bool _isActive;
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
    static void step(Task* me);

};
#endif