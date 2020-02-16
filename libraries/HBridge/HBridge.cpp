#include "Arduino.h"
#include "HBridge.h"

HBridge::HBridge(int pinDirA, int pinDirB, int pinEnable) {
  _pinDirA = pinDirA;
  _pinDirB = pinDirB;
  _pinEnable = pinEnable;

}
void HBridge::init() {
  pinMode(this->_pinDirA, OUTPUT);
  pinMode(this->_pinDirB, OUTPUT);
  pinMode(this->_pinEnable, OUTPUT);
}

void HBridge::setSpeed(int speed) {
  this->_speed = speed;
  if (speed < 0) {
    digitalWrite(this->_pinDirA, HIGH);
    digitalWrite(this->_pinDirB, LOW);

  } else {
    digitalWrite(this->_pinDirA, LOW);
    digitalWrite(this->_pinDirB, HIGH);
  }
  switch (abs(speed)) {
    case 0:
      digitalWrite(this->_pinEnable, LOW);
      break;
    case 255:
      digitalWrite(this->_pinEnable, HIGH);
      break;
    default:
      analogWrite(this->_pinEnable, abs(speed));
      break;
  }
}

int HBridge::getSpeed() {
  return this->_speed;
}