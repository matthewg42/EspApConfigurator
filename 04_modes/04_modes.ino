#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <MutilaDebug.h>
#include <Heartbeat.h>
#include <Mode.h>
#include "APButton.h"
#include "HeartBeat.h"
#include "NormalMode.h"
#include "APMode.h"
#include "Config.h"

Mode* BaseMode = NULL;

void switchMode(Mode* newMode)
{
    if (BaseMode!=NULL) {
        BaseMode->stop();
    }
    BaseMode = newMode;
    BaseMode->start();
}

void setup() {
    Serial.begin(115200);
    HeartBeat.begin();
    APButton.begin();
    switchMode(&NormalMode);
    DBLN(F("E:setup"));
}

void loop() {
    APButton.update();
    HeartBeat.update();
    if (BaseMode) BaseMode->update();

    // Handle BaseMode mode changes
    // #1 Press button for >= 2 seconds, go into AP mode
    if (APButton.held(2000)) {
        for (uint8_t i=0; i<8; i++) {
            digitalWrite(HEARTBEAT_PIN, LOW);
            delay(70);
            digitalWrite(HEARTBEAT_PIN, HIGH);
            delay(70);
        }
        switchMode(&APMode);
    }
}

