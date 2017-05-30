#include <ESP8266WiFi.h>
#include <Heartbeat.h>
#include <MutilaDebug.h>

Heartbeat heartbeat(D5);
const char *ssid = "ESP AP";

void setup() {
    Serial.begin(115200);
    delay(50);
    heartbeat.begin();
    DBLN(F("\n\nS:setup"));
    DB("Configuring access point...");
    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    DB("AP IP address: ");
    DBLN(myIP);
    DBLN(F("E:setup"));
}

void loop() {
    heartbeat.update();
}

