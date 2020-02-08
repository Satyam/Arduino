#ifndef POTENCIOMETRO_H
#define POTENCIOMETRO_H

#include "Arduino.h"
#include "Task.h"

class Potenciometro : public Task {
  public:
    Potenciometro(int pin, void (*onHandler)(int value));
    void start();
    void stop();
    bool isActive();
    int getValue();
    void setPin(int pin);
    void setHandler(void (*onHandler)(int value));
    void setPollingInterval(unsigned long ms);

  private:
    bool _isInitialized;
    int _pin;
    int _idle;
    int _last;
    bool _isActive;
    void (*_onHandler)(int value);
    int _readPotenciometro();
    static void step(Task* me);
};

#endif
