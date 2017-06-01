#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include <DebouncedButton.h>
#include <Millis.h>
#include "PersistentSettingAtom.h"

// Goal: read write different types to/from eeprom on the ESP8266

bool isEven(unsigned long n)
{
    return (n%2) == 0;
}

DebouncedButton button(D6);
PersistentSettingAtom<unsigned long> intSetting(0, 3142, &isEven);

void getIt()
{
    DB("intSetting.get()   = ");
    DBLN(intSetting.get());
    DB("intSetting.load()  = ");
    DBLN(intSetting.load());
    DB("intSetting.get()   = ");
    DBLN(intSetting.get());
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
}

/*
const uint16_t _size = 24;
const uint16_t _address = 5;
const char _default = 0;

void getIt()
{
    DBLN("getIt");

    // Init
    char* buf = new char[_size];
    memset(buf, _default, sizeof(char)*_size);

    // read from EEPROM
    for (uint16_t offset=0; offset<_size; offset++) {
        EEPROM.get(_address+offset, buf[offset]);
    }

    // display
    DB("got: ");
    DBLN(buf);
    delete buf;
}

void setIt() 
{
    DBLN("setIt");
    // Init
    char* buf = new char[_size];
    memset(buf, _default, sizeof(char)*_size);

    // Set value
    snprintf(buf, _size, "sz=%d, set at %ld", Millis());

    for (uint16_t offset=0; offset<_size; offset++) {
        EEPROM.put(_address+offset, buf[offset]);
    }
    // de-alloc buf
    delete buf;
    EEPROM.commit();
} */

void setup()
{
    Serial.begin(115200);
    delay(100);
    button.begin();
    DBLN("E:setup");
    EEPROM.begin(512);

    DB("before loading intSetting = ");
    DBLN(intSetting.get());
    DB("intsetting.load           = ");
    DBLN(intSetting.load());
    DB("then intSetting is        = ");
    DBLN(intSetting.get());
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

