/*
 * BloomBoysFirmware
 * Firmware for dissolved oxygen depth profile sensor
 */

#include "motor.h"
#include "sensor.h"

// sampling depths in feet
const int numDepths = 2;
float depthsFeet[numDepths] = {0.5, 1};
int depths[numDepths];
// samping frequency in seconds
const unsigned freq = 20;

// global variables
Motor motor(4, D1);
//Sensor sensor(97);
int measurements[numDepths];
unsigned nextProfile = 0;

// setup serial & register cloud functions
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numDepths; ++i)
    depths[i] = depthsFeet[i] * motor.clicksPerFeet;
  // use as encoder Vcc
  pinMode(D5, OUTPUT);
  pinSetFast(D5);
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
    delay(500);
    //measurements[i] = sensor.measure();
  }
  motor.runTo(0);
}

// print profile to serial
void show() {
  Serial.print("\n" + Time.timeStr(nextProfile - freq) + "\n");
  Serial.print("Depth (ft): DO (mg/L)\n");
  Serial.print("-----------------------\n");
  for (int i = 0; i < numDepths; ++i) {
    Serial.printf("%10f: %9f\n", depthsFeet, measurements[i]);
  }
}

// send profile to influxdb
// figure out sane schema
/*void send() {
  String line = "do value=%f";
  request.body = String::format(line, measurements[0]);
  client.post(request, response, headers);
}*/
