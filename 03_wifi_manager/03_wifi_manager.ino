#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <DebouncedButton.h>
#include <MutilaDebug.h>
#include <Heartbeat.h>
#include "Config.h"

Heartbeat heartbeat(D5);
DebouncedButton button(D6);

void doAPMode()
{ 
    DBLN(F("S:doAPMode"));
    WiFiManager wifiManager;
    //wifiManager.resetSettings();  // for testing (clears EEPROM-saved credentials?)
    wifiManager.setTimeout(120);
    if (!wifiManager.startConfigPortal("MouseESP", "pewppewp")) {
        DBLN(F("Connection fail/timeout"));
        delay(3000);
        ESP.reset();
        delay(5000);
    }
    DBLN(F("E:doAPMode (connected)"));
}

void setup() {
    Serial.begin(115200);
    heartbeat.begin();
    button.begin();
    DBLN(F("E:setup"));
}

void loop() {
    button.update();
    heartbeat.update();
    if (button.tapped()) doAPMode();
}

