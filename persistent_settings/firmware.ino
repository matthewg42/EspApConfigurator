#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingLong.h"
#include "PersistentSettingString.h"
#include "PersistentSettingManager.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

DebouncedButton button(D6);

PersistentSettingLong longSetting(0, 3142);
PersistentSettingString strSetting(sizeof(unsigned long), STRSIZE);
PersistentSettingLong longSetting2(sizeof(unsigned long)+STRSIZE-1, 1234);
PersistentSettingString strSetting2(100, STRSIZE);
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
    unsigned long v = random(0,400);
    DB("longSetting.set("); 
    DB(v);
    DB(") = ");
    DBLN(longSetting.set(String(v)));
    DB("longSetting.save() = "); 
    DBLN(longSetting.save());
    DBLN();

    DB("longSetting2.set("); 
    DB(v);
    DB(") = ");
    DBLN(longSetting2.set(String(v)));
    DB("longSetting2.save() = "); 
    DBLN(longSetting2.save());
    DBLN();

    char buf[STRSIZE+1];
    memset(buf, 0, STRSIZE+1);
    snprintf(buf, STRSIZE+1, "m=%ld abcdefghjklmnopqrstu", Millis());
    DB("strSetting.set("); 
    DB(buf);
    DB(") = ");
    DBLN(strSetting.set(buf));
    DB("strSetting.save()  = "); 
    DBLN(strSetting.save());
    DBLN();
    DBLN();
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN("\n\nS:setup");
    button.begin();
    EEPROM.begin(512);
    settings.addSetting(String("Some Random Long Int"), (PersistentSetting*)(&longSetting));
    settings.addSetting(String("Last Pressed Message"), (PersistentSetting*)(&strSetting));
    settings.addSetting(String("Another Long Int Set"), (PersistentSetting*)(&longSetting2));
    settings.addSetting(String("One Too many setting"), (PersistentSetting*)(&strSetting2));
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

