#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "MyAPI.h"
#include "Config.h"

ESP8266WebServer MyAPI(WEB_SERVER_PORT);

void MyAPI_begin()
{
    DBLN(F("MyAPI_begin"));
    MyAPI.on("/", MyAPI_handleRoot);
    MyAPI.on("/disconnect", MyAPI_handleDisconnect);
    MyAPI.onNotFound(MyAPI_handleNotFound);
    MyAPI.begin();
}

void MyAPI_handleRoot()
{
    DBLN(F("MyAPI_handleRoot"));
    MyAPI.send(200, "text/plain", "hello from esp8266!");
}

void MyAPI_handleDisconnect()
{
    DBLN(F("MyAPI_handleDisconnect"));
    MyAPI.send(200, "text/plain", "Disconnecting from WiFi...");
    WiFi.disconnect();
}

void MyAPI_handleNotFound()
{
    DBLN(F("MyAPI_handleNotFound"));
    String message = "URI Not Found\n\n";
    message += "URI: ";
    message += MyAPI.uri();
    message += "\nMethod: ";
    message += (MyAPI.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += MyAPI.args();
    message += "\n";
    for (uint8_t i=0; i<MyAPI.args(); i++){
        message += " " + MyAPI.argName(i) + ": " + MyAPI.arg(i) + "\n";
    }
    MyAPI.send(404, "text/plain", message);
}


