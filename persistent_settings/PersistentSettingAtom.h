#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>

/*! \brief EEPROM-backed non-volatile storage of atomic (non-array) settings
 */
template <class T>
class PersistentSettingAtom {
public:
    //! syntactic simplication for passing validators
    typedef bool (*validatorFunction)(T);

public:
    /*! \brief Constructor
     *
     *  Note that load() is not called in the constructor - the setting will have defaultValue on creation.
     *
     *  \param eepromAddress the FIRST byte in EEPROM which is used by this setting
     *  \param defaultValue the default to be used when first created or after loading when loaded value is not valid
     *  \param validator a function pointer to a function used to validate loaded / set values
     *
     *  TODO: Think of a graceful way to handle default values which do not pass validation
     */
    PersistentSettingAtom(uint16_t eepromAddress, T defaultValue, validatorFunction validator=NULL);

    /*! \brief get the current value
     *
     *  \return the value of the setting - note: may not be the same as the saved value.
     */
    T get();

    /*! \brief set the value
     *
     *  If the specified value fails the isValid(newValue) test, the value of the setting will remain unchanged.
     *  This only sets the value in RAM - to save to EEPROM, save() must be called.
     *
     *  \return true if the specified value was set, else false
     */
    bool set(T newValue);

    /*! \brief Validation
     *
     *  Determine if the current value is valid - used after loading from EEPROM and when setting the value. If
     *  there is no validator function specified for this setting (i.e. validator is NULL), any value is consifered
     *  valid.  If validator is not NULL, it will used to determine validite of the value being tested.
     *  valid/invalid value.
     *
     *  \return true if value is valid, else false
     */
    bool isValid(T value);

protected:
    uint16_t _address;
    T _default;
    validatorFunction _validator;
    T _value;

};

template <class T>
PersistentSettingAtom<T>::PersistentSettingAtom(uint16_t eepromAddress, T defaultValue, validatorFunction validator) : 
    _address(eepromAddress),
    _default(defaultValue),
    _validator(validator)
{
    _value = _default;
}

template <class T>
T PersistentSettingAtom<T>::get() 
{
    return _value;
}

template <class T>
bool PersistentSettingAtom<T>::set(T newValue) 
{
    if (isValid(newValue)) {
        _value = newValue;
        return true;
    } else {
        return false;
    }
}

template <class T>
bool PersistentSettingAtom<T>::isValid(T value) 
{
    if (_validator == NULL) {
        return true;
    } else {
        return _validator(value);
    }
}


