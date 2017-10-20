#include <EEPROM.h>
#include <MutilaDebug.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PersistentSettingUInt8.h>
#include <PersistentSettingChar.h>
#include <PersistentSettingBool.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingString.h>
#include <EspApConfigurator.h>
#include <ModeWifiClient.h>

#define AP_BUTTON_PIN       D7
#define HEARTBEAT_PIN       D0

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));

    EspApConfigurator.begin(AP_BUTTON_PIN, 
                            HEARTBEAT_PIN,
                            true,                       // Invert heartbeat logic for NodeMCU build-in LED
                            HttpServer_::MultiplePage); // Choose to use Multi-page web interface

    ModeWifiClient.enableHttpServer(true);              // Enable web interface in WiFi client mode

    // Example settings of each type
    EspApConfigurator.addSetting("Long Int Setting",    new PersistentSettingLong(EspApConfigurator.nextFreeAddress(),   987654321));
    EspApConfigurator.addSetting("Float Setting",       new PersistentSettingFloat(EspApConfigurator.nextFreeAddress(),  3.14159265359, 6));
    EspApConfigurator.addSetting("Char Setting",        new PersistentSettingChar(EspApConfigurator.nextFreeAddress(),   'M'));
    EspApConfigurator.addSetting("Bool Setting",        new PersistentSettingBool(EspApConfigurator.nextFreeAddress(),   true));
    EspApConfigurator.addSetting("UInt8 Setting",       new PersistentSettingUInt8(EspApConfigurator.nextFreeAddress(),  42));
    EspApConfigurator.addSetting("String Setting",      new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 16, "string(16)"));

    // Dump settings to serial
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

