#pragma once

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <EEPROM.h>

/*! \brief EEPROM-backed non-volatile storage of array settings
 *
 *  Most likely used for char strings
 */
template <class T>
class PersistentSettingArray {
public:
    //! syntactic simplication for passing validators
    typedef bool (*validatorFunction)(T*, uint16_t length);

public:
    /*! \brief Constructor
     *
     *  Note that load() is not called in the constructor - the setting will have defaultValue on creation.
     *
     *  \param eepromAddress the FIRST byte in EEPROM which is used by this setting
     *  \param defaultValue the default to set EACH ELEMENT to (e.g. each character in a string - usually 0)
     *  \param length the maximum number of elements in the array
     *  \param validator a function pointer to a function used to validate loaded / set values
     *
     *  TODO: Think of a graceful way to handle default values which do not pass validation
     */
    PersistentSettingArray(uint16_t eepromAddress, T defaultValue, uint16_t length, validatorFunction validator=NULL);

    /*! \brief destructor
     *
     *  We need one since we're using dynamic allocation... egads, have mercy on us!
     */
    ~PersistentSettingArray();

    /*! \brief get the current value
     *
     *  \return the value of the setting - note: may not be the same as the saved value.
     */
    const T* get();

    /*! \brief set the value
     *
     *  If the specified value fails the isValid() test, the value of the setting will remain unchanged.
     *  This only sets the value in RAM - to save to EEPROM, save() must be called.
     *
     *  \param newValue the source to copy from
     *  \param length the number of elements to copy - note all other elements will be set to the default value
     *
     *  \return true if the specified value was set, else false
     */
    bool set(T* newValue, uint16_t length=0);

    /*! \brief Load the value stored in EEPROM for this setting.
     *
     *  \return true if the value loaded from EEPROM passes validation by isValid(), else false.
     */
    bool load();

    /*! \brief Save the value to EEPROM
     *
     *  \return true if the value was saved OK, else false
     */
    bool save();

    /*! \brief Get the number of elements in the array
     */
    uint16_t length();

    /*! \brief Get the number of bytes of EEPROM used by this setting
     *
     *  \return size in bytes
     */
    size_t size();

    /*! \brief Validation
     *
     *  Determine if the current value is valid - used after loading from EEPROM and when setting the value. If
     *  there is no validator function specified for this setting (i.e. validator is NULL), any value is consifered
     *  valid.  If validator is not NULL, it will used to determine validite of the value being tested.
     *  valid/invalid value.
     *
     *  \return true if value is valid, else false
     */
    bool isValid(T* value, uint16_t length=0);

    /*! \brief peek at what is in EEPROM for this setting (may not be a valid value)
     *
     *  \param buf the buffer to be populated with the value from EEPROM - must be pre-allocated
     *  \param length the number of elements to check - if 0, check all elements.
     * 
     *  \return the value in EEPROM for this setting
     */
    void peek(T* buf);

protected:
    uint16_t _address;
    T _default;
    uint16_t _length;
    validatorFunction _validator;
    T* _value;

    //! used to clear _value and temporary buffers
    void clear(T*);
    //! copying between buffers
    void copy(T* src, T* dest, uint16_t length);
};

template <class T>
PersistentSettingArray<T>::PersistentSettingArray(uint16_t eepromAddress, T defaultValue, uint16_t length, validatorFunction validator) :
    _address(eepromAddress),
    _default(defaultValue),
    _length(length),
    _validator(validator)
{
    _value = new T[_length];
    clear(_value);
}

template <class T>
PersistentSettingArray<T>::~PersistentSettingArray() 
{
    delete _value;
    _value = NULL;
}

template <class T>
const T* PersistentSettingArray<T>::get() 
{
    return _value;
}

template <class T>
bool PersistentSettingArray<T>::set(T* newValue, uint16_t length) 
{
    // sanitize length
    if (length == 0 || length > _length) {
        length = _length;
    }

    if (isValid(newValue, length)) {
        copy(newValue, _value, length);
        return true;
    } else {
        return false;
    }
}

template <class T>
bool PersistentSettingArray<T>::isValid(T* value, uint16_t length) 
{
    if (_validator == NULL) {
        return true;
    } else {
        // sanitize length
        if (length == 0 || length > _length) {
            length = _length;
        }
        return _validator(value, length);
    }
}

template <class T>
bool PersistentSettingArray<T>::load()
{
    T* loaded = new T[_length];
    peek(loaded);

    if (isValid(loaded)) {
        copy(loaded, _value, _length);
        delete loaded;
        return true;
    } else {
        delete loaded;
        return false;
    }
}

template <class T>
bool PersistentSettingArray<T>::save()
{
    // TODO: non-ESP code
    for (uint16_t offset=0; offset<_length; offset++) {
        EEPROM.put(_address+offset, _value[offset]);
    }
    EEPROM.commit();
    T* loaded = new T[_length];
    peek(loaded);
    bool matches = true;
    for (uint16_t i=0; i<_length; i++) {
        if (loaded[i] != _value[i]) {
            matches = false;
            break;
        }
    }
    delete loaded;
    return matches;
}

template <class T>
size_t PersistentSettingArray<T>::size()
{
    return sizeof(T) * _length;
}

template <class T>
uint16_t PersistentSettingArray<T>::length()
{
    return _length;
}

template <class T>
void PersistentSettingArray<T>::peek(T* buf)
{
    // TODO: non-ESP code

    for (uint16_t offset=0; offset<_length; offset++) {
        EEPROM.get(_address+offset, buf[offset]);
    }
}

template <class T>
void PersistentSettingArray<T>::clear(T* buf)
{
    for (uint16_t i=0; i<_length; i++) {
        buf[i] = _default;
    }
}

template <class T>
void PersistentSettingArray<T>::copy(T* src, T* dest, uint16_t length)
{
    if (length == 0 || length > _length) {
        length = _length;
    }

    for (uint16_t i=0; i<_length; i++) {
        if (i<length) { dest[i] = src[i]; }
        else { dest[i] = _default; }
    }
}


