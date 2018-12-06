/*
 * BloomBoysFirmware
 * Firmware for dissolved oxygen depth profile sensor
 */

#include "motor.h"
#include "sensor.h"
#include "influx.h"
#include "config.h"

// sampling depths in feet
const int numDepths = 6;
float depthsFeet[numDepths] = {0, 5, 10, 15, 20, 25};
int depths[numDepths];
// samping frequency in seconds
unsigned freq = 240;

// global variables
Motor motor(4, D3);
Sensor sensor(97);
Influx db(
  INFLUX_HOST, 8086, "BLOOMBOYS",
  INFLUX_USER, INFLUX_PASSWORD
);
float measurements[numDepths];
unsigned nextProfile = 0;

// setup serial & register cloud functions
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numDepths; ++i)
    depths[i] = depthsFeet[i] * motor.clicksPerFeet;
  // use D5 as encoder Vcc
  pinMode(D5, OUTPUT);
  pinSetFast(D5);
  Particle.function("offset", offset);
}

// collect & send depth profile data repeatedly
void loop() {
  unsigned now = Time.now();
  if (now < nextProfile) return;
  nextProfile = now + freq;
  profile();
}

int offset(String depth) {
  motor.runTo(depth.toInt());
  motor.setPos(0);
  return 0;
}

// collect depth profile
void profile() {
  for (int i = 0; i < numDepths; ++i) {
    motor.runTo(depths[i]);
    measurements[i] = sensor.measure();
  }
  db.send(numDepths, depthsFeet, measurements);
  motor.runTo(-1);
}

// print profile to serial
void show() {
  Serial.print("\n" + Time.timeStr(nextProfile - freq) + "\n");
  Serial.print("Depth (ft): DO (mg/L)\n");
  Serial.print("-----------------------\n");
  for (int i = 0; i < numDepths; ++i) {
    Serial.printf("%10f: %9f\n", depthsFeet[i], measurements[i]);
  }
}
