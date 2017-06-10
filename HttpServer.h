#pragma once

#include <ESP8266WebServer.h>

class HttpServer_ : public ESP8266WebServer {
public:
    HttpServer_(int port=80);
    void begin();

};

extern HttpServer_ HttpServer;
