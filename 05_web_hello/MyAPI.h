#pragma once

#include <ESP8266WebServer.h>

extern ESP8266WebServer MyAPI;

void MyAPI_begin();
void MyAPI_handleRoot();
void MyAPI_handleDisconnect();
void MyAPI_handleNotFound();

