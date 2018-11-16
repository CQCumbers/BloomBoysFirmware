#include "sensor.h"
#include "Particle.h"

Sensor::Sensor(int addr_in): addr(addr_in) {
  Wire.begin();
}

float Sensor::measure() {
  // transmit code to sensor
  Wire.beginTransmission(addr);
  Wire.write("c");
  Wire.endTransmission();

  delay(600);
  Wire.requestFrom(addr, 20);
  if (Wire.read() != 1)
    printf("measurement failed");

  return Wire.parseFloat();
}
