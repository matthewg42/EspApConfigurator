#include <DNSServer.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PersistentSettingUInt8.h>
#include <PersistentSettingChar.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingString.h>
#include <EspApConfigurator.h>
#include <ModeWifiClient.h>

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));

    EspApConfigurator.begin();
    ModeWifiClient.enableHttpServer(true);

    // Example settings of each type
    EspApConfigurator.addSetting("Long Int Setting",    new PersistentSettingLong(EspApConfigurator.nextFreeAddress(),   987654321));
    EspApConfigurator.addSetting("Float Setting",       new PersistentSettingFloat(EspApConfigurator.nextFreeAddress(),  3.14159265359, 6));
    EspApConfigurator.addSetting("Char Setting",        new PersistentSettingChar(EspApConfigurator.nextFreeAddress(),   'M'));
    EspApConfigurator.addSetting("UInt8 Setting",       new PersistentSettingUInt8(EspApConfigurator.nextFreeAddress(),  42));
    EspApConfigurator.addSetting("String Setting",      new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 16, "string(16)"));

    // Dump settings
    DBLN(F("Settings:"));
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
        DB(EspApConfigurator[i].id);
        DB(F("="));
        DB(EspApConfigurator[i].setting->get());
        DB(F(" typecode="));
        DBLN(EspApConfigurator[i].setting->typecode());
        
    }

    DBLN(F("E:setup"));
}

void loop() 
{
    // Give timeslice
    EspApConfigurator.update();
}

