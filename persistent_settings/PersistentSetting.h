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
    virtual uint16_t size() = 0;
    uint16_t address() { return _address; }

protected:
    uint16_t _address;

};

