#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingLong.h"
#include "PersistentSettingString.h"
#include "PersistentSettingChar.h"
#include "PersistentSettingManager.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

DebouncedButton button(D6);

PersistentSettingManager settings(3);
PersistentSetting* longSetting;
PersistentSetting* charSetting;
PersistentSetting* strSetting;

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

    String s = "Millis=" + String(Millis());
    DB("strSetting->set(");
    DB(s);
    DB(")=");
    DB(strSetting->set(s));
    DB(", save()=");
    DBLN(strSetting->save());
    DBLN();

}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN("\n\nS:setup");
    button.begin();
    EEPROM.begin(512);
    
    longSetting =   settings.addSetting(
                        String("Long integer setting"),
                        new PersistentSettingLong(settings.nextFreeAddress(), 3142)
                    );
    charSetting =   settings.addSetting(
                        String("Character setting"),
                        new PersistentSettingChar(settings.nextFreeAddress(), 'c')
                    );
    strSetting =    settings.addSetting(
                        String("String setting"),
                        new PersistentSettingString(settings.nextFreeAddress(), STRSIZE)
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

