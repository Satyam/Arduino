#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include "Arduino.h"

class HBridge {
  public:
    HBridge(int pinDirA, int pinDirB, int pinEnable);
    void init();
    void speed(int speed);
  private:
    int _pinDirA;
    int _pinDirB;
    int _pinEnable;
};
#endif