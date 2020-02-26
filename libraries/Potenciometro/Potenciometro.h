#ifndef POTENCIOMETRO_H
#define POTENCIOMETRO_H

#include "Arduino.h"
#include "MyTask.h"

class Potenciometro : public MyTask {
  public:
    Potenciometro(int pin, void (*onHandler)(int value));
    int getValue();
    void setPin(int pin);
    void setHandler(void (*onHandler)(int value));
    void init() override;
    void step() override;

  private:
    int _pin;
    int _idle;
    int _last;
    void (*_onHandler)(int value);
    int _readPotenciometro();
};

#endif
