//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include <DNSServer.h>
//#include <WiFiManager.h>
#include <MutilaDebug.h>
#include <Heartbeat.h>
#include <Mode.h>
#include "APButton.h"
#include "HeartBeat.h"
#include "NormalMode.h"
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

//void doAPMode()
//{ 
//    DBLN(F("S:doAPMode"));
//    WiFiManager wifiManager;
//    //wifiManager.resetSettings();  // for testing (clears EEPROM-saved credentials?)
//    wifiManager.setTimeout(120);
//    if (!wifiManager.startConfigPortal(AP_NAME, AP_PASS)) {
//        DBLN(F("Connection fail/timeout"));
//        delay(3000);
//        ESP.reset();
//        delay(5000);
//    }
//    DBLN(F("E:doAPMode (connected)"));
//}

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
    //if (APButton.tapped()) doAPMode();
    if (BaseMode) BaseMode->update();
}

