#include <Arduino.h>
#include <MutilaDebug.h>
#include <ESP8266WiFi.h>
#include "ModeReset.h"
#include "MutilaDebug.h"
#include "Config.h"

ModeReset_ ModeReset;

ModeReset_::ModeReset_() :
    NamedMode("ModeReset")
{
}

void ModeReset_::modeStart()
{
    DBLN(F("ModeReset::modeStart()"));
    WiFi.disconnect(true);
    for (uint8_t i=0; i<33; i++) {
        digitalWrite(HEARTBEAT_PIN, i%2);
        delay(15);
    }
    digitalWrite(HEARTBEAT_PIN, 0);
    delay(500);
    ESP.reset();
}


