#include <EEPROM.h>
#include <MutilaDebug.h>
#include "PersistentSettingBool.h"

bool basicBoolValidator(String s)
{
    return s == "0" || s == "1";
}

PersistentSettingBool::PersistentSettingBool(uint16_t eepromAddress, bool defaultValue, validatorFunction validator) :
    PersistentSettingAtom<bool>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingBool::load()
{
    bool loaded = peek();
    String s(loaded);
    if (isValid(s)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingBool::set(String newValue)
{
    if (isValid(newValue) && newValue.length() == 1) {
        _value = newValue[0] != '0';
        return true;
    } else {
        return false;
    }   
}

