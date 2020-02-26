#ifndef MY_TASK_H
#define MY_TASK_H

#include "Arduino.h"
#include "Task.h"

class MyTask : public Task {
  public:
    MyTask(unsigned long interval);
    void setPollingInterval(unsigned long ms);
    virtual void start();
    virtual void stop();
    bool isActive();
    virtual void step() {};
  private:
    volatile bool _isActive;
    static void _step(Task* me);
};
#endif