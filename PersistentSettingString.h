#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PersistentSetting.h"

// Check that strings are made only of printing ASCII characters
bool basicStringValidator(String s);

/*! \brief EEPROM-backed non-volatile storage of atomic (non-array) settings
 */
class PersistentSettingString : public PersistentSetting {
public:
    //! syntactic simplication for passing validators
    typedef bool (*validatorFunction)(String);

public:
    /*! \brief Constructor
     *
     *  Note that load() is not called in the constructor - the setting will have defaultValue on creation.
     *
     *  \param eepromAddress the FIRST byte in EEPROM which is used by this setting
     *  \param maxLength the maximum number of characters in the string
     *  \param defaultValue the default to be used when first created or after loading when loaded value is not valid
     *  \param validator a function pointer to a function used to validate loaded / set values
     *
     *  TODO: Think of a graceful way to handle default values which do not pass validation
     */
    PersistentSettingString(uint16_t eepromAddress, uint16_t maxLength, String defaultValue, validatorFunction validator=&basicStringValidator);

    /*! \brief get the current value
     *
     *  \return the value of the setting - note: may not be the same as the saved value.
     */
    String get();

    /*! \brief set the value
     *
     *  If the specified value fails the isValid(newValue) test, the value of the setting will remain unchanged.
     *  This only sets the value in RAM - to save to EEPROM, save() must be called.
     *
     *  \return true if the specified value was set, else false
     */
    bool set(String newValue);

    /*! \brief Load the value stored in EEPROM for this setting.
     *
     * \param newValue the new value to set (pending validation)
     *
     *  \return true if the value loaded from EEPROM passes validation by isValid(), else false.
     */
    bool load();

    /*! \brief Save the value to EEPROM
     *
     *  \return true if the value was saved OK, else false
     */
    bool save();

    /*! \brief Get the number of bytes of EEPROM used by this setting
     *
     *  \return size in bytes
     */
    uint16_t size();
    uint16_t formLength() { return size(); }

    /*! \brief Reset the value in RAM to the default value
     */
    virtual void reset() { _value = _defaultValue; }

    /*! \brief Validation
     *
     *  Determine if the current value is valid - used after loading from EEPROM and when setting the value. If
     *  there is no validator function specified for this setting (i.e. validator is NULL), any value is consifered
     *  valid.  If validator is not NULL, it will used to determine validite of the value being tested.
     *  valid/invalid value.
     *
     *  \return true if value is valid, else false
     */
    bool isValid(String value);

    /*! \brief peek at what is in EEPROM for this setting (may not be a valid value)
     * 
     *  \return the value in EEPROM for this setting
     */
    String peek();

    String typecode();

protected:
    uint16_t _maxLength;
    String _defaultValue;
    validatorFunction _validator;
    String _value;

};

