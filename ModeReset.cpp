#include <Arduino.h>
#include <MutilaDebug.h>
#include <ESP8266WiFi.h>
#include "ModeReset.h"
#include "MutilaDebug.h"
#include "Config.h"
#include "EspApConfigurator.h"

ModeReset_ ModeReset;

ModeReset_::ModeReset_()
{
}

void ModeReset_::modeStart()
{
    DBLN(F("ModeReset::modeStart()"));

    // Reset settings and save to EEPROM
    EspApConfigurator.resetAll();

    // Disconnect from network and discard remembered SSID/pass
    WiFi.disconnect(true);

    // show we're doing something with the LED
    for (uint8_t j=0; j<3; j++) {
        for (uint8_t i=0; i<33; i++) {
            digitalWrite(HEARTBEAT_PIN, i%2);
            delay(30);
        }
        digitalWrite(HEARTBEAT_PIN, 0);
        delay(500);
    }

    // reset the device (both of these cause a hang sometimes)
    ESP.reset();
    // or: ESP.restart();
}


