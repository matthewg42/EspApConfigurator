#pragma once

#include <Arduino.h>
#include "PersistentSetting.h"

class settingPair {
public:
    settingPair() : id(String()), setting(NULL) {;}
    String id;
    PersistentSetting* setting;
};

class PersistentSettingManager {
public:
    PersistentSettingManager(uint8_t maxSettings);
    ~PersistentSettingManager();
    PersistentSetting* addSetting(String id, PersistentSetting* setting, bool load=true);
    uint8_t count();
    settingPair operator[](uint8_t idx);

    /*! Get next free EEPROM address
     *
     * Free as in not-known to be in use by PersistentSettingManager
     *
     */
    uint16_t nextFreeAddress();

private:
    uint8_t _maxSettings;
    uint8_t _count;
    settingPair* _settings;

};

