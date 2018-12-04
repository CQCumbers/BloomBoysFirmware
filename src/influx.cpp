#include "influx.h"
#include "Particle.h"

Influx::Influx(String host, int port, String db, String user, String pass) {
  request.hostname = host;
  request.port = port;
  request.path = String::format(path, db.c_str(), user.c_str(), pass.c_str(), "s");
}

void Influx::send(int size, float *depths, float *measurements) {
  int timestamp = Time.now();
  request.body = "";
  for (int i = 0; i < size; ++i) {
    request.body += String::format(
      send_fmt, *(depths + i), *(measurements + i), timestamp
    );
  }
  headers[2].value = String(request.body.length());
  http.post(request, response, headers);
  Serial.printf("Response Status: %i\n", response.status);
  // TODO: handle response.status != 400
}
