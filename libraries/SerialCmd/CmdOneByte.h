#ifndef CMD_ONE_BYTE_H
#define CMD_ONE_BYTE_H

#include "Arduino.h"
#include "MyTask.h"

class CmdOneByte : public MyTask {
  public:
    CmdOneByte(void (*onHandler)(char cmd, byte value));
    void setNoArgCmds( char * list);
    void init() override;
    void step() override;
  private:
    char _cmd;
    char *_noArgList;
    void (*_onHandler)(char cmd, byte value);
};
#endif