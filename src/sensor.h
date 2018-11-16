#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
private:
  int addr = 97;

public:
  Sensor(int addr_in);
  float measure();
};

#endif
