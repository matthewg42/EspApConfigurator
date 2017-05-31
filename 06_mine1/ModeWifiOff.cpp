#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeWifiOff.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"

ModeWifiOff_ ModeWifiOff;

ModeWifiOff_::ModeWifiOff_() :
    NamedMode("ModeWifiOff")
{
    setUpdatePeriod(500);
}

void ModeWifiOff_::modeStart()
{
    DBLN(F("ModeWifiOff::modeStart()"));
    WiFi.forceSleepBegin(); 
    HeartBeat.setMode(Heartbeat::Slow);
}

void ModeWifiOff_::modeEnd()
{
    DBLN(F("ModeWifiOff::modeStart()"));
    WiFi.forceSleepWake();
}

void ModeWifiOff_::modeUpdate()
{
    DBLN(F("ModeWifiOff::modeUpdate()"));
}

