#include <Arduino.h>
#include <EEPROM.h>
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

void PersistentSettingManager::begin()
{
    EEPROM.begin(512);
}

PersistentSetting* PersistentSettingManager::addSetting(const char* id, PersistentSetting* setting, bool load) 
{
    addSetting(String(id), setting, load);
}

PersistentSetting* PersistentSettingManager::addSetting(String id, PersistentSetting* setting, bool load) 
{
    if (_count >= _maxSettings) {    
        DBLN(F("ERROR maxSettings reached"));
        return NULL;
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
            DB(F(" start="));
            DB(newFirst);
            DB(F(" last="));
            DB(newLast);
            DB(F(" / "));
            DB(_settings[i].id);
            DB(F(" start="));
            DB(oldFirst);
            DB(F(" last="));
            DBLN(oldLast);
            return NULL;
        }
    }

    if (load) { setting->load(); }
    _settings[_count].id = id;
    _settings[_count].setting = setting;
    _count++;
    return setting;
}

uint8_t PersistentSettingManager::count()
{
    return _count;
}

settingPair PersistentSettingManager::operator[](uint8_t idx)
{
    return _settings[idx];
}

String PersistentSettingManager::operator[](const char* id)
{
    return this->operator[](String(id));
}

String PersistentSettingManager::operator[](String id)
{
    for (uint8_t i=0; i<_count; i++) {
        if (_settings[i].id == id) {
            return _settings[i].setting->get();
        }
    }
    return String();
}

uint16_t PersistentSettingManager::nextFreeAddress()
{
    uint16_t lastUsedAddress = 0;
    for (uint8_t i=0; i<_count; i++) {
        uint16_t last  = _settings[i].setting->address() + _settings[i].setting->size()-1;
        if (last >= lastUsedAddress) {
            lastUsedAddress = last;
        }
    }
    return lastUsedAddress+1;
}

