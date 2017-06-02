#include <EEPROM.h>
#include "PersistentSettingLong.h"

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

