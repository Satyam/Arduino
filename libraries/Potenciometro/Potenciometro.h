#ifndef POTENCIOMETRO_H
#define POTENCIOMETRO_H

#include "Arduino.h"
#include "Task.h"

#define POLLING_TIME_MS 50

class Potenciometro : public Task {
  public:
    Potenciometro(int pin, void (*onHandler)(int value));
    void begin();
    int getValue();
    void setPin(int pin);
    void setHandler(void (*onHandler)(int value));

  private:
    int _pin;
    int _idle;
    int _last;
    void (*_onHandler)(int value);
    int _readPotenciometro();
    static void step(Task* me);
};

#endif
