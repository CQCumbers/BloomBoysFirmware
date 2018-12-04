#ifndef INFLUX_H
#define INFLUX_H

#include "HttpClient.h"

class Influx {
private:
  HttpClient http;
  http_header_t headers[5] = {
    {"User-Agent", "Photon/1.0"},
    {"Accept", "*/*"},
    {"Content-Length", "32"},
    {"Content-Type", "application/x-www-form-urlencoded"},
    {0, 0} 
  };
  http_request_t request;
  http_response_t response;

  const String path = "/write?db=%s&u=%s&p=%s&precision=%s";
  const String send_fmt = "do,depth=%f value=%f %i\n";

public:
  Influx(String host, int port, String db, String user, String pass);
  void send(int size, float *depths, float *measurements);
};

#endif
