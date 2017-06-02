#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "PersistentSettingAtom.h"

class PersistentSettingLong : public PersistentSettingAtom<long> {
public:
    PersistentSettingLong(uint16_t eepromAddress, long defaultValue, validatorFunction validator=NULL);
    bool load();
    bool set(String newValue);

};

PersistentSettingLong::PersistentSettingLong(uint16_t eepromAddress, long defaultValue, validatorFunction validator) :
    PersistentSettingAtom<long>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingLong::load()
{
    long loaded = peek();
    String s(loaded);
    if (isValid(s)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingLong::set(String newValue)
{
    if (isValid(newValue)) {
        _value = newValue.toInt();
        return true;
    } else {
        return false;
    }   
}

