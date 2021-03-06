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

void HttpServer_::begin(HttpServer_::Mode mode)
{
    DB(F("HttpServer::begin "));

    switch (mode) {
    case HttpServer_::SinglePage:
        DBLN(F("SinglePage"));
        onNotFound(handleNotFound);
        on("/", handleSinglePage);
        on("/save", handleSingleSave);
        on("/discard", handleSingleCancel);
        on("/r", handleRescan);
        break;
    case HttpServer_::MultiplePage:
        DBLN(F("MultiplePage"));
        on("/", handleLandingPage);
        on("/wifi", handleWifiPage);
        on("/set", handleSettingsPage);
        on("/savewifi", handleWifiSave);
        on("/saveset", handleSettingsSave);
        on("/cancel", handleCancel);
        on("/r", handleRescan);
        onNotFound(handleNotFound);
        break;
    default:
        DBLN(F("Custom"));
        // We don't set up any routes here - the sketch is responsible now
    };
    ESP8266WebServer::begin();
}



