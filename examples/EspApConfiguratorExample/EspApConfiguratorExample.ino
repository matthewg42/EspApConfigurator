#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MutilaDebug.h>
#include <DNSServer.h>
#include <EspApConfigurator.h>
#include <PersistentSettingString.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingChar.h>

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));
    EspApConfigurator.begin();

    // Must add settings AFTER begin()
    EspApConfigurator.addSetting("The number PI",  new PersistentSettingFloat(EspApConfigurator.nextFreeAddress(), 3.1415926));
    EspApConfigurator.addSetting("String setting", new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 32, "string value"));

    DBLN(F("E:setup"));
}

void loop() 
{
    // Give timeslice
    EspApConfigurator.update();
}

