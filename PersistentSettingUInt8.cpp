#include <EEPROM.h>
#include "PersistentSettingUInt8.h"

PersistentSettingUInt8::PersistentSettingUInt8(uint16_t eepromAddress, uint8_t defaultValue, validatorFunction validator) :
    PersistentSettingAtom<uint8_t>(eepromAddress, defaultValue, validator)
{
}

bool PersistentSettingUInt8::load()
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

bool PersistentSettingUInt8::set(String newValue)
{
    if (isValid(newValue)) {
        _value = newValue.toInt();
        return true;
    } else {
        return false;
    }   
}

