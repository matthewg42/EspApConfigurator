#include <ESP8266WiFi.h>
#include <MutilaDebug.h>

// Misc local includes
#include "NamedMode.h"

// Some globally visible object instances
#include "APButton.h"
#include "HeartBeat.h"

// Top level modes
#include "ModeAPBase.h"
#include "ModeWifiOff.h"

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
    ModeAPBase.begin();
    ModeWifiOff.begin();
    switchMode(&ModeWifiOff);
    DBLN(F("E:setup"));
}

void loop() 
{
    HeartBeat.update();
    APButton.update();
    pBaseMode->update();

    if (APButton.held(2000)) {
        APButton.setState(false);
        switchMode(&ModeAPBase);
    }
}

