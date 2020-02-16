#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include "Arduino.h"

class HBridge {
  public:
    HBridge(int pinDirA, int pinDirB, int pinEnable);
    void init();
    void setSpeed(int speed);
    int getSpeed();
  private:
    int _pinDirA;
    int _pinDirB;
    int _pinEnable;
    int _speed;
};
#endif