#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <MutilaDebug.h>
#include "APMode.h"
#include "MutilaDebug.h"
#include "Config.h"

APMode_ APMode;

APMode_::APMode_() 
{
    setUpdatePeriod(500);
}

void APMode_::modeStart()
{
    DBLN(F("APMode::modeStart()"));
    WiFiManager wifiManager;
    //wifiManager.resetSettings();  // for testing (clears EEPROM-saved credentials?)
    wifiManager.setTimeout(120);
    if (!wifiManager.startConfigPortal(AP_NAME, AP_PASS)) {
        DBLN(F("Connection fail/timeout"));
        delay(3000);
        ESP.reset();
        delay(5000);
    }
    DBLN(F("E:APMode::modeStart"));
}

void APMode_::modeUpdate()
{
    DBLN(F("APMode::modeUpdate()"));
}

