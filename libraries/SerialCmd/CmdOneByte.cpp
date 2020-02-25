#include "Arduino.h"
#include "CmdOneByte.h"
#include "SoftTimer.h"

CmdOneByte::CmdOneByte(void (*onHandler)(char cmd, byte value)): Task(0, &(CmdOneByte::step))  {
  _onHandler = onHandler;  
};

void CmdOneByte::setNoArgCmds( char * list) {
  this->_noArgList = list;
};

void CmdOneByte::setPollingInterval(unsigned long ms) {
  this->setPeriodMs(ms);
};

void CmdOneByte::start() {
  SoftTimer.add(this);
  this->_isActive = true;
}

void CmdOneByte::stop() {
  SoftTimer.remove(this);
  this->_isActive = false;
}

bool CmdOneByte::isActive() {
  return this->_isActive;
};

void CmdOneByte::init() {
  this->_cmd = NULL;
  Task::init();
};

void CmdOneByte::step(Task* task) {
  CmdOneByte* me = (CmdOneByte*)task;

  if (Serial.available()) {
    char cmd = Serial.read();
    if (me->_cmd) {
      me->_onHandler(me->_cmd, (byte)cmd);
      me->_cmd = NULL;
    } else {
      if (strchr(me->_noArgList, cmd)) {
        me->_onHandler(cmd, 0);
      } else {
        me->_cmd = cmd;
      }
    }
  }
};

