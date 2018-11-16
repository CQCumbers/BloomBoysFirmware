#ifndef MOTOR_H
#define MOTOR_H

#include "PhoBot.h"

class Motor {
private:
  PhoBot phobot;
  int enc1, enc2;
  int state, lastState;
  volatile int pos = 0;
  volatile int stop_pos = pos;
  void interrupt();

public:
  Motor(int enc1, int enc2);
  void runTo(int pos_in);
  void stop();
  void setPos(int pos_in);
  int getPos() { return pos; }
};

#endif
