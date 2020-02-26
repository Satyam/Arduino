#include "Arduino.h"
#include "CmdOneByte.h"
#include "SoftTimer.h"

CmdOneByte::CmdOneByte(void (*onHandler)(char cmd, byte value)): MyTask(0)  {
  _onHandler = onHandler;  
};

void CmdOneByte::setNoArgCmds( char * list) {
  this->_noArgList = list;
};


void CmdOneByte::init() {
  this->_cmd = NULL;
  MyTask::init();
};

void CmdOneByte::step() {

  if (Serial.available()) {
    char cmd = Serial.read();
    if (this->_cmd) {
      this->_onHandler(this->_cmd, (byte)cmd);
      this->_cmd = NULL;
    } else {
      if (strchr(this->_noArgList, cmd)) {
        this->_onHandler(cmd, 0);
      } else {
        this->_cmd = cmd;
      }
    }
  }
};

