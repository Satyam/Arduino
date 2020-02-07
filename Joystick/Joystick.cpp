#include "Joystick.h"
#include <Task.h>

Joystick::Joystick(
  int xPin, void (*onXHandler)(int x),
  int yPin, void (*onYHandler)(int y),
  int swPin, void (*onButtonHandler)()
): Debouncer (swPin, MODE_CLOSE_ON_PUSH, onButtonHandler, NULL, true) {

  _xPin = xPin;
  _yPin = yPin;
  _swPin = swPin;
  _onXHandler = onXHandler;
  _onYHandler = onYHandler;

  Serial.println("instantiated Joystick");
};


void Joystick::init()
{
  PciManager.registerListener(this->_swPin, this);
  this->_xyTask = new Task(XY_POLLING_TIME_MS, &(Joystick::step));


  Debouncer::init();

  this->_idleX = this->_readJoystick(this->_xPin, 0);
  this->_idleY = this->_readJoystick(this->_yPin, 0);

  SoftTimer.add(_xyTask);
}

int Joystick::_readJoystick(int pin, int idle) {
  return (analogRead(pin) >> 1) - idle;
}

int Joystick::getX() {
  return this->_lastX;
}

int Joystick::getY() {
  return this->_lastY;
};

void Joystick::setXYPolling(unsigned long ms) {
  this->_xyTask->setPeriodMs(ms);;
}

void Joystick::step(Task* task) {
  Joystick* me = (Joystick*)task;
  int x = me->_readJoystick(me->_xPin, me->_idleX);

  Serial.println("-------------");  
  Serial.print("x: ");
  Serial.println(x);
  Serial.print("_idleX: ");
  Serial.println(me->_idleX);
  Serial.print("_lastX: ");
  Serial.println(me->_lastX);
  Serial.print("_onXHandler is NULL: ");
  Serial.println(me->_onXHandler == NULL);

  if (x != me->_lastX) {
    me->_lastX = x;
    if (me->_onXHandler != NULL) me->_onXHandler(x);
  }

  int y = me->_readJoystick(me->_yPin, me->_idleY);
  Serial.print("y: ");
  Serial.println(y);
  Serial.print("_idleY: ");
  Serial.println(me->_idleY);
  Serial.print("_lastY: ");
  Serial.println(me->_lastY);
  Serial.print("_onYHandler is NULL: ");
  Serial.println(me->_onYHandler == NULL);

  if (y != me->_lastY) {
    me->_lastY = y;
    if (me->_onYHandler != NULL) me->_onYHandler(y);
  }

}
