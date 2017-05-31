#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeAPServe.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"
#include "Config.h"

ModeAPServe_ ModeAPServe;

ModeAPServe_::ModeAPServe_() :
    NamedMode("ModeAPServe"),
    apIP(192, 168, 4, 1),
    netMsk(255, 255, 255, 0)
{
    setUpdatePeriod(500);
}

void ModeAPServe_::modeStart()
{
    DBLN(F("S:ModeAPServe::modeStart()"));
    HeartBeat.setMode(Heartbeat::Quick);
    DB("Access point SSID=");
    DB(AP_NAME);
    WiFi.softAP(AP_NAME, AP_PASS);
    IPAddress myIP = WiFi.softAPIP();
    DB(" AP IP address: ");
    DBLN(myIP);
    DBLN(F("E:ModeAPServe::modeStart()"));
}

void ModeAPServe_::modeUpdate()
{
    DBLN(F("ModeAPServe::modeUpdate()"));
}

