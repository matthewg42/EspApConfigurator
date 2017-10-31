#pragma once

#include <Arduino.h>
#include "PersistentSetting.h"
#include "PersistentSettingUInt8.h"

class SettingPair {
public:
    SettingPair() : id(String()), setting(NULL) {;}
    String id;
    PersistentSetting* setting;
};

/*! Manager for PersistentSettings
 *
 * NOTE: PersistentSetting objects are stored in non-volatile flash on the ESP8266 / ESP32.
 * However, because the interface to that storage is defined in the EEPROM.h header in
 * the ESP API, and usage semantics are broadly similar to the Arduino EEPROM API,
 * backing store will be referred to as EEPROM in this documentation.
 *
 * This class stores multiple PersistentSetting objects, providing the ability to access
 * a setting by it's ID. The first time a given set of settings is used on a phyical device,
 * the values in EEPROM may not match valid values. To help prevent junk settings, this
 * class calculates a signature for the settings and stores this in the first byte of 
 * EEPROM.  The checkSignature() member can be used to see if the signature of settings
 * in memory matches EEPROM, and if not, reset the settings.
 *
 * The signature byte is stored at address 0.  Other settings should use nextFreeAddress()
 * to get an address.
 */

class PersistentSettingManager {
public:
    /*! Constructor
     *
     * \param maxSettings the maximum number of settings this manager can hold
     */
    PersistentSettingManager(uint8_t maxSettings);

    /*! Destructor
     */
    ~PersistentSettingManager();

    /*! Initialization
     */
    void begin();

    /*! Add a setting to the manager
     *
     * \param id the unique identifier for this setting, e.g. "NTP server"
     * \param setting a pointer to the PersistentSetting to be managed
     * \param load if true, load the setting's value from EEPROM
     * \return setting on success, else NULL
     *
     * This function can fail for a few reasons:
     * 1. The maximum number of settings has already been used
     * 2. The address of the setting in EEPROM overlaps another setting managed by this object
     * 3. The id is not unique
     *
     */
    PersistentSetting* addSetting(String id, PersistentSetting* setting, bool load=true);

    /*! Add a setting to the manager
     *
     * Over-loader to allow char* id.
     */
    PersistentSetting* addSetting(const char* id, PersistentSetting* setting, bool load=true);

    /*! Get number currently managed settings
     *
     * \return the number of settings currently managed by this object
     */
    uint8_t count();

    /*! Get a SettingPair using the index (between 0 and count())
     *
     * \param idx the index of the SettingPair to be retrieved
     * \return the SettingPair object at index idx
     */
    SettingPair operator[](uint8_t idx);

    /*! Get the PersistentSetting with the specified id
     *
     * \param id the id of the setting to retrieve
     * \return the PersistentSetting with the specified id, 
     *         or NULL of no setting has the specified id.
     */
    PersistentSetting* operator[](String id);

    /*! Overloaded to allow use of const char* id.
     */
    PersistentSetting* operator[](const char* id);

    /*! Reset all settings to default and save
     */
    void resetAll();

    /*! Get next free EEPROM address
     *
     * Free as in not-known to be in use by PersistentSettingManager
     */
    uint16_t nextFreeAddress();

    /*! Check signture matches saved value.
     *  
     *  \return true if the current settings in memory match the signature in EEPROM
     */
    bool checkSignature();

protected:
    /*! Calculate signature based on current setting names
     */
    uint8_t calculateSignature();

private:
    uint8_t _maxSettings;
    uint8_t _count;
    PersistentSettingUInt8 _signature;
    SettingPair* _settings;

};

