#include <Arduino.h>
#include <MutilaDebug.h>
#include "PersistentSettingManager.h"

PersistentSettingManager::PersistentSettingManager(uint8_t maxSettings) :
    _maxSettings(maxSettings),
    _count(0)
{
    _settings = new settingPair[_maxSettings];
}

PersistentSettingManager::~PersistentSettingManager()
{
    delete _settings;
    _settings = NULL;
}

void PersistentSettingManager::addSetting(String id, PersistentSetting* setting, bool load) 
{
    if (_count < _maxSettings) {    
        if (load) { setting->load(); }
        _settings[_count].id = id;
        _settings[_count].setting = setting;
        _count++;
    } else { 
        DBLN(F("ERROR maxSettings reached"));
    }
}

uint8_t PersistentSettingManager::count()
{
    return _count;
}

settingPair PersistentSettingManager::operator[](uint8_t idx)
{
    return _settings[idx];
}

