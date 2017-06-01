#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingArray.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

bool isEven(unsigned long n)
{
    return (n%2) == 0;
}

DebouncedButton button(D6);
PersistentSettingArray<char> strSetting(0, 0, STRSIZE);

void getIt()
{
    DB("strSetting.get()   = ");
    DBLN(strSetting.get());
    DB("strSetting.load()  = ");
    DBLN(strSetting.load());
    DB("strSetting.get()   = ");
    DBLN(strSetting.get());
    DBLN();
}

void setIt()
{
    char buf[STRSIZE];
    memset(buf, 0, STRSIZE);
    snprintf(buf, STRSIZE, "m=%ld", Millis());
    DB("strSetting.set("); 
    DB(buf);
    DB(") = ");
    DBLN(strSetting.set(buf));
    DB("strSetting.save() = "); 
    DBLN(strSetting.save());
    DBLN();
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    button.begin();
    DBLN("E:setup");
    EEPROM.begin(512);

    DB("before loading strSetting = ");
    DBLN(strSetting.get());
    DB("strSetting.load           = ");
    DBLN(strSetting.load());
    DB("then strSetting is        = ");
    DBLN(strSetting.get());
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

