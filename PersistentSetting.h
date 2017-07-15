#pragma once

#include <stdint.h>
#include <Arduino.h>

/*! \brief Abstract base class for PersistentSetting* classes of various types
 */
class PersistentSetting {
public:
    /*! \brief Constructor
     *
     *  Note that load() is not called in the constructor
     *
     *  \param eepromAddress the FIRST byte in EEPROM which is used by this setting
     */
    PersistentSetting(uint16_t eepromAddress) : _address(eepromAddress) {;}

    /*! \brief Get the current value as a String
     *
     *  \return the value of the setting in RAM, as represented in a String.
     */
    virtual String get() = 0;

    /*! \brief Set the value from a String
     *
     *  If the specified value fails the isValid(newValue) test, the value of the setting will remain unchanged.
     *  This only sets the value in RAM - to save to EEPROM, save() must be called.
     *
     *  \return true if the specified value was set, else false
     */
    virtual bool set(String newValue) = 0;

    /*! \brief Load the value stored in EEPROM into RAM
     *
     * \param newValue the new value to set (pending validation)
     *
     *  \return true if the value loaded from EEPROM passes validation by isValid(), else false.
     */
    virtual bool load() = 0;

    /*! \brief Save the value in RAM to EEPROM
     *
     *  \return true if the value was saved OK, else false
     */
    virtual bool save() = 0;

    /*! \brief Get the number of bytes of EEPROM used by this setting
     *
     *  \return size in bytes
     */
    virtual uint16_t size() = 0;

    /*! \brief Get the maximum length in characters
     * 
     *  \return max length in characters when represented as a string
     */
    virtual uint16_t formLength() = 0;

    /*! \brief Reset the value in RAM to the default value
     */
    virtual void reset() = 0;

    /*! \brief Get the EEPROM address of the first byte for this setting
     * 
     *  \return max length in characters when represented as a string
     */
    uint16_t address() { return _address; }

    /*! Get a typecode which will tell the web interface how to render this setting */
    virtual String typecode() = 0;

protected:
    uint16_t _address;

};

