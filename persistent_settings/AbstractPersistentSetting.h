#pragma once

#include <stdint.h>
#include <Arduino.h>

/*! \brief Base class for PersistentSetting*
 */
template <class T>
class AbstractPersistentSetting {
public:
    AbstractPersistentSetting() {;}
    virtual T get() = 0;
    virtual bool set(T newValue) = 0;
    virtual bool load() = 0;
    virtual bool save() = 0;
    virtual size_t size() = 0;

};

