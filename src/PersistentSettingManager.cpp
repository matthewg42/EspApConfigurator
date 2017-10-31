#include <Arduino.h>
#include <EEPROM.h>
#include <MutilaDebug.h>
#include "PersistentSettingManager.h"

PersistentSettingManager::PersistentSettingManager(uint8_t maxSettings) :
    _maxSettings(maxSettings),
    _count(0),
    _signature(0, 0)
{
    _settings = new SettingPair[_maxSettings];
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
        if (_settings[i].id == id) {
            DB(F("ERROR: two settings have the same id: "));
            DBLN(id);
            return NULL;
        }

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

PersistentSetting* PersistentSettingManager::addSetting(const char* id, PersistentSetting* setting, bool load) 
{
    addSetting(String(id), setting, load);
}

uint8_t PersistentSettingManager::count()
{
    return _count;
}

SettingPair PersistentSettingManager::operator[](uint8_t idx)
{
    return _settings[idx];
}

PersistentSetting* PersistentSettingManager::operator[](const char* id)
{
    return this->operator[](String(id));
}

PersistentSetting* PersistentSettingManager::operator[](String id)
{
    for (uint8_t i=0; i<_count; i++) {
        if (_settings[i].id == id) {
            return _settings[i].setting;
        }
    }
    return NULL;
}

uint16_t PersistentSettingManager::nextFreeAddress()
{
    uint16_t lastUsedAddress = _signature.size();
    for (uint8_t i=0; i<_count; i++) {
        uint16_t last  = _settings[i].setting->address() + _settings[i].setting->size()-1;
        if (last >= lastUsedAddress) {
            lastUsedAddress = last;
        }
    }
    return lastUsedAddress+1;
}

void PersistentSettingManager::resetAll()
{
    for (uint8_t i=0; i<_count; i++) {
        _settings[i].setting->reset();
        _settings[i].setting->save();
    }
}

bool PersistentSettingManager::checkSignature()
{
    _signature.load();
    uint8_t signature = calculateSignature();
    if (signature == _signature.get().toInt()) {
        return true;
    } else {
        DBLN(F("PersistentSettingManager WARNING: signature doesn't match, resetting"));
        resetAll();
        _signature.set(String(signature));
        _signature.save();
        return false;
    }
}

uint8_t PersistentSettingManager::calculateSignature()
{
    // XOR bysubSig of id, address & typecode for all settings
    uint8_t signature = 0;
    for (uint8_t settingIndex=0; settingIndex<_count; settingIndex++) {
        uint8_t subSig = 0;
        uint8_t i;
        // add setting name
        for (i=0; i<_settings[settingIndex].id.length(); i++) {
            subSig ^= _settings[settingIndex].id[i];
        }

        // add setting type
        for (i=0; i<_settings[settingIndex].setting->typecode().length(); i++) {
            subSig ^= _settings[settingIndex].setting->typecode()[i];
        }

        // add setting address
        uint16_t address = _settings[settingIndex].setting->address();
        subSig ^= ((uint8_t*)&address)[0];
        subSig ^= ((uint8_t*)&address)[1];

        // the sig for this setting is multipled by it's index.  This way if 
        // settings are just re-ordered, that will trigger a sig fail
        signature ^= (subSig * settingIndex) % 256;
    }
    return signature;
}



