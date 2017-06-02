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
    void addSetting(String id, PersistentSetting* setting);
    uint8_t count();
    settingPair operator[](uint8_t idx);

private:
    uint8_t _maxSettings;
    uint8_t _count;
    settingPair* _settings;

};

