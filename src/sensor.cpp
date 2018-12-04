#include "sensor.h"
#include "Particle.h"

Sensor::Sensor(int addr_in): addr(addr_in) {
  Wire.begin();
}

float Sensor::measure() {
  // transmit code to sensor
  Wire.beginTransmission(addr);
  Wire.write("r");
  Wire.endTransmission();

  delay(600);
  Wire.requestFrom(addr, 20);
  if (Wire.read() != 1)
    Serial.print("measurement failed");

  return Wire.parseFloat();
}
