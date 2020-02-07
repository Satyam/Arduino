#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include <PciManager.h>
#include <SoftTimer.h>
#include <Debouncer.h>

#define XY_POLLING_TIME_MS 50

class Joystick : public Debouncer {
  public:
    Joystick(
      int xPin, void (*onXHandler)(int x),
      int yPin, void (*onYHandler)(int y),
      int swPin, void (*onButtonHandler)()
    );
    void init() override;
    int getX();
    int getY();
    void setXYPolling(unsigned long ms);

  private:
    int _xPin;
    int _yPin;
    int _swPin;
    int _idleX;
    int _idleY;
    int _lastX;
    int _lastY;
    void (*_onXHandler)(int x);
    void (*_onYHandler)(int y);
    int _readJoystick(int pin, int idle);
    Task* _xyTask;
    static void step(Task* me);
};

#endif
