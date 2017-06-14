#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include <Heartbeat.h>
#include <EspApConfigurator.h>
#include "ModeWifiOff.h"
#include "MutilaDebug.h"

ModeWifiOff_ ModeWifiOff;

ModeWifiOff_::ModeWifiOff_()
{
    setUpdatePeriod(500);
}

void ModeWifiOff_::modeStart()
{
    DBLN(F("ModeWifiOff::modeStart()"));
    WiFi.forceSleepBegin(); 
    EspApConfigurator.heartbeat()->setMode(Heartbeat::Slow);
}

void ModeWifiOff_::modeUpdate()
{
}

