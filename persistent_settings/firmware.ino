#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

#include "PersistentSettingLong.h"
#include "PersistentSettingString.h"
#include "PersistentSettingChar.h"
#include "PersistentSettingUInt8.h"
#include "PersistentSettingFloat.h"
#include "PersistentSettingManager.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

DebouncedButton button(D6);

PersistentSettingManager settings(5);
PersistentSetting* longSetting;
PersistentSetting* charSetting;
PersistentSetting* uint8Setting;
PersistentSetting* strSetting;
PersistentSetting* floatSetting;

void getIt()
{
    for(uint8_t i=0; i<settings.count(); i++) {
        DB(settings[i].id);
        DB(" = ");
        DBLN(settings[i].setting->get());
    }
    DB("charSetting as int: ");
    DBLN((int)charSetting->get()[0]);
    
    DBLN();
}

void setIt()
{
    long r = random (-16000000,16000000);
    DB("longSetting->set(");
    DB(r);
    DB(")=");
    DB(longSetting->set(String(r)));
    DB(", save()=");
    DBLN(longSetting->save());

    char c = random('A', 'Z');
    DB("charSetting->set(");
    DB(c);
    DB(")=");
    DB(charSetting->set(String(c)));
    DB(", save()=");
    DBLN(charSetting->save());

    uint8_t u = random(0, 255);
    DB("uint8Setting->set(");
    DB(u);
    DB(")=");
    DB(uint8Setting->set(String(u)));
    DB(", save()=");
    DBLN(uint8Setting->save());

    String s = "Millis=" + String(Millis());
    DB("strSetting->set(");
    DB(s);
    DB(")=");
    DB(strSetting->set(s));
    DB(", save()=");
    DBLN(strSetting->save());

    float f = r / 1000.;
    DB("floatSetting->set(");
    DB(f);
    DB(")=");
    DB(floatSetting->set(String(f)));
    DB(", save()=");
    DBLN(floatSetting->save());

    DBLN();
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN("\n\nS:setup");
    button.begin();
    EEPROM.begin(512);
    
    longSetting =   settings.add(
                        String("Long integer setting"),
                        new PersistentSettingLong(settings.nextFreeAddress(), 3142)
                    );
    charSetting =   settings.add(
                        String("Character setting"),
                        new PersistentSettingChar(settings.nextFreeAddress(), 'c')
                    );
    uint8Setting =  settings.add(
                        String("UInt8 setting"),
                        new PersistentSettingUInt8(settings.nextFreeAddress(), 42)
                    );
    strSetting =    settings.add(
                        String("String setting"),
                        new PersistentSettingString(settings.nextFreeAddress(), STRSIZE)
                    );
    floatSetting =  settings.add(
                        String("Float setting"),
                        new PersistentSettingFloat(settings.nextFreeAddress(), 3.142)
                    );
    
    DBLN("E:setup");
}

void loop()
{
    button.update();
    unsigned long t = button.tapped();
    if (t > 0) {
        if (t < 500) {
            getIt();
        } else { 
            setIt();
        }
    }
}

