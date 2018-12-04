#ifndef MOTOR_H
#define MOTOR_H

#include "PhoBot.h"

class Motor {
private:
  PhoBot phobot;
  int encoder;
  String stop, forward, backward;
  volatile int pos = 0;
  int stop_pos = 0;
  void interrupt();

public:
  static constexpr float clicksPerFeet = 1860;
  int direction = 1;
  volatile bool running = false;

  Motor(int enc1, int enc2);
  void runTo(int pos_in);
  void setPos(int pos_in);
  int getPos() { return pos; }
};

#endif
