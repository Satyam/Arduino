#include "Arduino.h"
#include "MyTask.h"
#include "SoftTimer.h"

MyTask::MyTask(unsigned long ms): Task(0, &(MyTask::_step))  {
  this->setPeriodMs(ms);
};

void MyTask::setPollingInterval(unsigned long ms) {
  this->setPeriodMs(ms);
};

void MyTask::start() {
  SoftTimer.add(this);
  this->_isActive = true;
}

void MyTask::stop() {
  SoftTimer.remove(this);
  this->_isActive = false;
}

bool MyTask::isActive() {
  return this->_isActive;
};

void MyTask::_step(Task * task) {
  MyTask* me = (MyTask*)task;
  me->step();
}