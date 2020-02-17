#ifndef CMD_ONE_BYTE_H
#define CMD_ONE_BYTE_H

#include "Arduino.h"
#include "Task.h"

class CmdOneByte : public Task {
  public:
    CmdOneByte(void (*onHandler)(char cmd, byte value));
    void setNoArgCmds( char * list);
    void setPollingInterval(unsigned long ms);
    void init() override;
    void start();
    void stop();
    bool isActive();
  private:
    bool _isActive;
    char _cmd;
    char *_noArgList;
    void (*_onHandler)(char cmd, byte value);
    static void step(Task* me);

};
#endif