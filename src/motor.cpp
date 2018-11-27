#include "motor.h"
#include "Particle.h"

Motor::Motor(int motor, int enc_in): encoder(enc_in) {
  phobot = PhoBot(5.0, 6.0);
  stop = "M" + String(motor) + "-S";
  forward = "M" + String(motor) + "-F-100";
  backward = "M" + String(motor) + "-B-100";

  // second encoder doesn't work on our hardware :(
  pinMode(encoder, INPUT);
  attachInterrupt(encoder, &Motor::interrupt, this, CHANGE);
}

void Motor::interrupt() {
  Serial.printf("%i ", pos);
  pos += direction;
  // brake slightly ahead of time
  if (pos + direction * 35 != stop_pos)
    return;
  phobot.setMotors(stop);
  running = false;
}

// run motor to (possibly past) encoder pos
void Motor::runTo(int pos_in) {
  stop_pos = pos_in;
  if (pos_in > pos) {
    phobot.setMotors(forward);
    direction = 1;
    running = true;
  } else if (pos_in < pos) {
    phobot.setMotors(backward);
    direction = -1;
    running = true;
  }
  while (running)
    Particle.process();
}

// stop motor & reset position
void Motor::setPos(int pos_in) {
  phobot.setMotors(stop);
  running = false;
  pos = pos_in;
  stop_pos = pos_in;
}
