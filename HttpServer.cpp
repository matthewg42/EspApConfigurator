#include <MutilaDebug.h>
#include "HttpServer.h"
#include "HttpHandlers.h"

HttpServer_ HttpServer(80);

HttpServer_::HttpServer_(int port) :
    ESP8266WebServer(port)
{
    DB(F("HttpServer::HttpServer port="));
    DBLN(port);
}

void HttpServer_::begin()
{
    DBLN(F("HttpServer::begin"));
    // Set up routes
    onNotFound(handleNotFound);
    on("/", handleRoot);
    on("/save", handleWifiSave);
    on("/wifi", handleWifi);
    on("/r", handleRescan);
    ESP8266WebServer::begin();
}



