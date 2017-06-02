#include <EEPROM.h>
#include "PersistentSettingString.h"

PersistentSettingString::PersistentSettingString(uint16_t eepromAddress, uint16_t maxLength, validatorFunction validator) : 
    PersistentSetting(eepromAddress),
    _maxLength(maxLength),
    _validator(validator)
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
    // TODO: non-ESP code
    for (uint16_t offset=0; offset<_maxLength; offset++) {
        char c;
        if (offset < _value.length()) {
            c = _value[offset];
        } else {
            c = 0;
        }
        EEPROM.put(_address+offset, c);
    }
    EEPROM.commit();
    return peek() == _value;
}

size_t PersistentSettingString::size()
{
    return _maxLength;
}

String PersistentSettingString::peek()
{
    // TODO: non-ESP code
    String loaded;
    for (uint16_t offset=0; offset<_maxLength; offset++) {
        char c;
        c = EEPROM.get(_address+offset, c);
        if (c != 0) {
            loaded += c;
        } else {
            break;
        }
    }
    return loaded;
}



