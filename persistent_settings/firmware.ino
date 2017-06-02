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

void getIt()
{
    for(uint8_t i=0; i<settings.count(); i++) {
        DB(settings[i].id);
        DB(" = ");
        DBLN(settings[i].setting->get());
    }
    DBLN();
}

void setIt()
{
    DBLN("setIt() NOTHING HERE");
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN("\n\nS:setup");
    button.begin();
    EEPROM.begin(512);
    settings.addSetting(String("Long integer setting"),
                        new PersistentSettingLong(settings.nextFreeAddress(), 3142));
    settings.addSetting(String("Character setting"),
                        new PersistentSettingChar(settings.nextFreeAddress(), 'c'));
    settings.addSetting(String("String setting"),
                        new PersistentSettingString(settings.nextFreeAddress(), STRSIZE));
    
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

