#include <MutilaDebug.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HttpServer.h"
#include "HttpHandlers.h"

void handleNotFound() {
    DBLN(F("handleNotFound"));
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += pHttpServer->uri();
    message += "\nMethod: ";
    message += ( pHttpServer->method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += pHttpServer->args();
    message += "\n";

    for ( uint8_t i = 0; i < pHttpServer->args(); i++ ) {
        message += " " + pHttpServer->argName ( i ) + ": " + pHttpServer->arg ( i ) + "\n";
    }
    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->send ( 404, "text/plain", message );
}

void handleRoot()
{
    DBLN(F("handleRoot"));
    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
    pHttpServer->send(200, "text/html", "");
    pHttpServer->sendContent("<html><head></head><body><h1>EspApConfigurator</h1><ul><li><a href=\"/settings\">Settings</li><li><a href=\"/wifi\">Wifi</li></ul></body></html>");
    pHttpServer->client().stop();
}

void handleSettingsPage()
{
    DBLN(F("handleSettingsPage"));
    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
    pHttpServer->send(200, "text/html", "");
    pHttpServer->sendContent("<html><head></head><body><h1>Settings</h1></body></html>");
    pHttpServer->client().stop();
}

void handleWifiPage()
{
    DBLN(F("handleWifiPage"));
    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
    pHttpServer->send(200, "text/html", "");
    pHttpServer->sendContent("<html><head></head><body><h1>Wifi Setup</h1><h2>Networks</h2><ul>");

    int8_t netCount = WiFi.scanComplete();
    for (int8_t i=0; i<netCount; i++) {
        String desc = WiFi.SSID(i);
        desc += ", Ch:";
        desc += String(WiFi.channel(i));
        desc += " (";
        desc += String(WiFi.RSSI(i));
        desc += "dBm) ";
        desc += WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "";
        pHttpServer->sendContent("<li>");
        pHttpServer->sendContent(desc.c_str());
        pHttpServer->sendContent("</li>");
    }
    pHttpServer->sendContent("</body></html>");
    pHttpServer->client().stop();
}

