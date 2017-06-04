#include <EEPROM.h>
#include "PersistentSettingString.h"

bool basicStringValidator(String s)
{
    for (uint8_t i=0; i<s.length(); i++) {
        char c = s[i];
        if (c<32 || c>126) {
            // non-printing ASCII
            return false;
        }
    }
    return true;
}

PersistentSettingString::PersistentSettingString(uint16_t eepromAddress, uint16_t maxLength, String defaultValue, validatorFunction validator) : 
    PersistentSetting(eepromAddress),
    _maxLength(maxLength),
    _defaultValue(defaultValue),
    _validator(validator),
    _value(defaultValue)
{
}

String PersistentSettingString::get() 
{
    return _value;
}

bool PersistentSettingString::set(String newValue) 
{
    if (isValid(newValue)) {
        _value = newValue;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingString::isValid(String value) 
{
    if (value.length() > _maxLength) {
        return false;
    }

    if (_validator == NULL) {
        return true;
    } else {
        return _validator(value);
    }
}

bool PersistentSettingString::load()
{
    String loaded = peek();
    if (isValid(loaded)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingString::save()
{
    for (uint16_t offset=0; offset<_maxLength; offset++) {
        uint8_t c;
        if (offset < _value.length()) {
            c = _value[offset];
        } else {
            c = 0;
        }
        EEPROM.write(_address+offset, c);
    }
    EEPROM.commit();
    String saved = peek();
    bool ok = (saved==_value);
    return ok;
}

uint16_t PersistentSettingString::size()
{
    return _maxLength;
}

String PersistentSettingString::peek()
{
    String loaded;
    for (uint16_t offset=0; offset<_maxLength; offset++) {
        uint8_t c = EEPROM.read(_address+offset);
        if (c != 0) {
            loaded += (char)c;
        } else {
            break;
        }
    }
    return loaded;
}


