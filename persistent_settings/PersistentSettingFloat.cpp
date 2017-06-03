#include <EEPROM.h>
#include "PersistentSettingFloat.h"

PersistentSettingFloat::PersistentSettingFloat(uint16_t eepromAddress, float defaultValue, validatorFunction validator) :
    PersistentSettingAtom<float>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingFloat::load()
{
    float loaded = peek();
    String s(loaded);
    if (isValid(s)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

bool PersistentSettingFloat::set(String newValue)
{
    if (isValid(newValue)) {
        _value = newValue.toFloat();
        return true;
    } else {
        return false;
    }   
}

