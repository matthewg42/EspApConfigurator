#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingAtom.h"
#include "PersistentSettingArray.h"
#include "PersistentSettingString.h"

// Goal: read write different types to/from eeprom on the ESP8266

#define STRSIZE 16

bool isEven(unsigned long n)
{
    return (n%2) == 0;
}

DebouncedButton button(D6);

PersistentSettingAtom<unsigned long> intSetting(0, 3142, &isEven);
PersistentSettingArray<char> arrSetting(sizeof(unsigned long), 0, STRSIZE);
PersistentSettingString strSetting(sizeof(unsigned long) + STRSIZE, STRSIZE);

void getIt()
{
    DB("intSetting.get()   = ");
    DBLN(intSetting.get());
    DB("intSetting.load()  = ");
    DBLN(intSetting.load());
    DB("intSetting.get()   = ");
    DBLN(intSetting.get());
    DBLN();

    DB("arrSetting.get()   = ");
    DBLN(arrSetting.get());
    DB("arrSetting.load()  = ");
    DBLN(arrSetting.load());
    DB("arrSetting.get()   = ");
    DBLN(arrSetting.get());
    DBLN();

    DB("strSetting.get()   = ");
    DBLN(strSetting.get());
    DB("strSetting.load()  = ");
    DBLN(strSetting.load());
    DB("strSetting.get()   = ");
    DBLN(strSetting.get());
    DBLN();
    DBLN();
}

void setIt()
{
    unsigned long v = random(0,400);
    DB("intSetting.set("); 
    DB(v);
    DB(") = ");
    DBLN(intSetting.set(v));
    DB("intSetting.save() = "); 
    DBLN(intSetting.save());
    DBLN();

    char buf[STRSIZE];
    memset(buf, 0, STRSIZE);
    snprintf(buf, STRSIZE, "m=%ld", Millis());
    DB("arrSetting.set("); 
    DB(buf);
    DB(") = ");
    DBLN(arrSetting.set(buf));
    DB("arrSetting.save() = "); 
    DBLN(arrSetting.save());
    DBLN();

    DB("strSetting.set("); 
    DB(buf);
    DB(") = ");
    DBLN(strSetting.set(buf));
    DB("strSetting.save() = "); 
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

    DB("before loading intSetting = ");
    DBLN(intSetting.get());
    DB("intSetting.load           = ");
    DBLN(intSetting.load());
    DB("then intSetting is        = ");
    DBLN(intSetting.get());
    DBLN();

    DB("before loading arrSetting = ");
    DBLN(arrSetting.get());
    DB("arrSetting.load           = ");
    DBLN(arrSetting.load());
    DB("then arrSetting is        = ");
    DBLN(arrSetting.get());
    DBLN();

    DB("before loading strSetting = ");
    DBLN(strSetting.get());
    DB("strSetting.load           = ");
    DBLN(strSetting.load());
    DB("then strSetting is        = ");
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

