/*
 * BloomBoysFirmware
 * Firmware for dissolved oxygen depth profile sensor
 */

#include "motor.h"
#include "sensor.h"

// sampling depths in encoder clicks
const int numDepths = 4;
const int depths[numDepths] = {0, 1, 2, 3};
// encoder clicks per feet
const float clicksPerFeet = 10;//2.317 * 1120;
// samping frequency in seconds
const unsigned freq = 120;

// global variables
Motor motor(D5, D6);
//Sensor sensor(97);
int measurements[numDepths];
unsigned nextProfile = 0;

// setup serial & register cloud functions
void setup() {
  Serial.begin(9600);
}

// collect & send depth profile data repeatedly
void loop() {
  unsigned now = Time.now();
  if (now < nextProfile) return;
  nextProfile = now + freq;
  profile();
  show();
}

// collect depth profile
void profile() {
  for (int i = 0; i < numDepths; ++i) {
    motor.runTo(depths[i]);
    //measurements[i] = sensor.measure();
  }
  motor.runTo(0);
}

// print profile to serial
void show() {
  Serial.printf("\n%s\n", Time.timeStr(nextProfile - freq));
  Serial.printf("Depth (ft): DO (mg/L)\n");
  Serial.printf("-----------------------");
  for (int i = 0; i < numDepths; ++i) {
    float depth = depths[i] / clicksPerFeet;
    Serial.printf("%10f: %9f\n", depth, measurements[i]);
  }
}

// send profile to influxdb
// figure out sane schema
/*void send() {
  String line = "do value=%f";
  request.body = String::format(line, measurements[0]);
  client.post(request, response, headers);
}*/
