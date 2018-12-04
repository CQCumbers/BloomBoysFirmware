/*
 * BloomBoysFirmware
 * Firmware for dissolved oxygen depth profile sensor
 */

#include "motor.h"
#include "sensor.h"
#include "influx.h"
#include "config.h"

// sampling depths in feet
const int numDepths = 4;
float depthsFeet[numDepths] = {5, 10, 15, 20};
int depths[numDepths];
// samping frequency in seconds
unsigned freq = 120;

// global variables
Motor motor(4, D3);
Sensor sensor(97);
Client client(
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
}

// collect & send depth profile data repeatedly
void loop() {
  unsigned now = Time.now();
  if (now < nextProfile) return;
  nextProfile = now + freq;
  profile();
}

// collect depth profile
void profile() {
  for (int i = 0; i < numDepths; ++i) {
    motor.runTo(depths[i]);
    measurements[i] = sensor.measure();
  }
  db.send(numDepths, depthsFeet, measurements);
  motor.runTo(0);
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
