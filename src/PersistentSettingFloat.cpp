#include <EEPROM.h>
#include "PersistentSettingFloat.h"

PersistentSettingFloat::PersistentSettingFloat(uint16_t eepromAddress, float defaultValue, uint8_t dp, validatorFunction validator) :
    PersistentSettingAtom<float>(eepromAddress, defaultValue, validator),
    _dp(dp)
{
}

bool PersistentSettingFloat::load()
{
    float loaded = peek();
    String s(loaded, _dp);
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

String PersistentSettingFloat::get()
{
    return String(_value, _dp);
}

String PersistentSettingFloat::typecode()
{
    String t = "f";
    t += String(_dp);
    return t;
}

