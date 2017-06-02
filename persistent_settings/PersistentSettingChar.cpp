#include <EEPROM.h>
#include "PersistentSettingChar.h"

PersistentSettingChar::PersistentSettingChar(uint16_t eepromAddress, uint8_t defaultValue, validatorFunction validator) :
    PersistentSettingAtom<uint8_t>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingChar::load()
{
    uint8_t loaded = peek();
    String s(loaded);
    if (isValid(s)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingChar::set(String newValue)
{
    if (isValid(newValue)) {
        _value = newValue.toInt();
        return true;
    } else {
        return false;
    }   
}

