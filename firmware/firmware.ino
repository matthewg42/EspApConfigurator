#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MutilaDebug.h>
#include <DNSServer.h>

// Misc local includes
#include "NamedMode.h"

// Some globally visible object instances
#include "APButton.h"
#include "HeartBeat.h"

// Top level modes
#include "ModeAP.h"
#include "ModeWifiClient.h"

// Compile time config options
#include "Config.h"

NamedMode* pBaseMode = NULL;

void switchMode(NamedMode* newMode)
{
    if (pBaseMode!=NULL) {
        pBaseMode->stop();
    }
    pBaseMode = newMode;
    pBaseMode->start();
    DB("BaseMode is now: ");
    DBLN(pBaseMode->name());
}

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));
    HeartBeat.begin();
    APButton.begin();
    ModeAP.begin();
    ModeWifiClient.begin();
    switchMode(&ModeWifiClient);
    DBLN(F("E:setup"));
}

void loop() 
{
    HeartBeat.update();
    APButton.update();
    pBaseMode->update();

    if (pBaseMode == &ModeAP) {
        if (pBaseMode->isFinished()) {
            switchMode(&ModeWifiClient);
        }
    } else {
        unsigned long tapMs = APButton.tapped();
        if (tapMs) {
            switchMode(&ModeAP);
        }
    }
}

