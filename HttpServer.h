#pragma once

#include <ESP8266WebServer.h>

class HttpServer_ : public ESP8266WebServer {
public:
    enum Mode {
        SinglePage,
        MultiplePage,
        Custom
    };
    HttpServer_(int port=80);
    void begin(Mode mode);

};

extern HttpServer_ HttpServer;
