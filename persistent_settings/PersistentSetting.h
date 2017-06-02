#pragma once

#include <stdint.h>
#include <Arduino.h>

/*! \brief Abstract base class for PersistentSetting* classes of various types
 */
class PersistentSetting {
public:
    PersistentSetting(uint16_t eepromAddress) : _address(eepromAddress) {;}
    virtual String get() = 0;
    virtual bool set(String newValue) = 0;
    virtual bool load() = 0;
    virtual bool save() = 0;
    virtual size_t size() = 0;

protected:
    uint16_t _address;

};

