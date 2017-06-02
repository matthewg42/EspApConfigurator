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

bool PersistentSettingManager::addSetting(String id, PersistentSetting* setting, bool load) 
{
    if (_count >= _maxSettings) {    
        DBLN(F("ERROR maxSettings reached"));
        return false;
    }

    // Check for EEPROM address conflicts in previous settings
    uint16_t newFirst = setting->address();
    uint16_t newLast  = setting->address() + setting->size()-1;
    for (uint8_t i=0; i<_count; i++) {
        uint16_t oldFirst = _settings[i].setting->address();
        uint16_t oldLast  = _settings[i].setting->address() + _settings[i].setting->size()-1;
        
        if ((newFirst>=oldFirst && newFirst<=oldLast) || (newLast>=oldFirst && newLast<=oldLast)) {
            DB(F("WARNING: EEPROM setting overlap: "));
            DB(id);
            DB('/');
            DBLN(_settings[i].id);
            return false;
        }
    }

    if (load) { setting->load(); }
    _settings[_count].id = id;
    _settings[_count].setting = setting;
    _count++;
    return true;
}

uint8_t PersistentSettingManager::count()
{
    return _count;
}

settingPair PersistentSettingManager::operator[](uint8_t idx)
{
    return _settings[idx];
}

