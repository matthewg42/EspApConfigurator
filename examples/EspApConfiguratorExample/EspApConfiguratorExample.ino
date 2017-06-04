#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MutilaDebug.h>
#include <DNSServer.h>
#include <EspApConfigurator.h>

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));
    EspApConfigurator.begin();
    DBLN(F("E:setup"));
}

void loop() 
{
    EspApConfigurator.update();
}

