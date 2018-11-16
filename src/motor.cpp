#include "motor.h"
#include "Particle.h"

Motor::Motor(int enc1, int enc2): enc1(enc1), enc2(enc2) {
  phobot = PhoBot(9.0, 6.0);
  pinMode(enc1, INPUT);
  pinMode(enc2, INPUT);

  state = lastState = (pinReadFast(enc1) << 1) | pinReadFast(enc2);
  attachInterrupt(enc1, &Motor::interrupt, this, CHANGE);
  attachInterrupt(enc2, &Motor::interrupt, this, CHANGE);
}

// adapted from https://github.com/monkbroc/Encoder
void Motor::interrupt() {
	state = (pinReadFast(enc1) << 1) | pinReadFast(enc2);
	switch ((lastState << 2) | state) {
	case 0: case 5: case 10: case 15:
		break;
	case 1: case 3: case 7:
  case 8: case 12: case 14:
		++pos;
    break;
	case 2: case 4: case 6:
  case 9: case 11: case 13:
		--pos;
    break;
	}
  lastState = state;
  if (pos != stop_pos) return;
  stop_pos = -1;
  phobot.setMotors("M4-S");
}

// run motor to encoder pos
void Motor::runTo(int pos_in) {
  stop_pos = pos_in;
  if (pos_in == pos)
    return;
  else if (pos_in > pos)
    phobot.setMotors("M4-F-100");
  else
    phobot.setMotors("M4-B-100");
}

// stop motor & reset position
void Motor::setPos(int pos_in) {
  phobot.setMotors("M4-S");
  pos = pos_in;
  stop_pos = pos_in;
}
