#include <EEPROM.h>
#include "PersistentSettingChar.h"

PersistentSettingChar::PersistentSettingChar(uint16_t eepromAddress, char defaultValue, validatorFunction validator) :
    PersistentSettingAtom<char>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingChar::load()
{
    char loaded = peek();
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
    if (isValid(newValue) && newValue.length() == 1) {
        _value = newValue[0];
        return true;
    } else {
        return false;
    }   
}

