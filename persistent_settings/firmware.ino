#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingLong.h"
#include "PersistentSettingString.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

bool isEven(String n)
{
    return (n.toInt()%2) == 0;
}

DebouncedButton button(D6);

PersistentSettingLong longSetting(0, 3142, &isEven);
PersistentSettingString strSetting(sizeof(unsigned long), STRSIZE);

void getIt()
{
    DB("longSetting.get()   = ");
    DBLN(longSetting.get());
    DB("longSetting.load()  = ");
    DBLN(longSetting.load());
    DB("longSetting.get()   = ");
    DBLN(longSetting.get());
    DBLN();

    DB("strSetting.get()    = ");
    DBLN(strSetting.get());
    DB("strSetting.load()   = ");
    DBLN(strSetting.load());
    DB("strSetting.get()    = ");
    DBLN(strSetting.get());
    DBLN();
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

    char buf[STRSIZE];
    memset(buf, 0, STRSIZE);
    snprintf(buf, STRSIZE, "m=%ld", Millis());
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
    button.begin();
    DBLN("E:setup");
    EEPROM.begin(512);

    DB("before loading longSetting = ");
    DBLN(longSetting.get());
    DB("longSetting.load           = ");
    DBLN(longSetting.load());
    DB("then longSetting is        = ");
    DBLN(longSetting.get());
    DBLN();

    DB("before loading strSetting  = ");
    DBLN(strSetting.get());
    DB("strSetting.load            = ");
    DBLN(strSetting.load());
    DB("then strSetting is         = ");
    DBLN(strSetting.get());
    DBLN();
    DBLN();
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

