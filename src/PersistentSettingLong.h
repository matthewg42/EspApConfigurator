#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PersistentSettingAtom.h"

/*! \brief Class for storing long int values to EEPROM
 */
class PersistentSettingLong : public PersistentSettingAtom<long> {
public:
    /*! Constructor
     * \param eepromAddress start address of this setting
     * \param defaultValue value to take before loading
     * \param validatorFunction function to validate values
     */
    PersistentSettingLong(uint16_t eepromAddress, long defaultValue, validatorFunction validator=NULL);

    /*! Load from EEPROM 
     *  Loads value from EEPROM, and checks for validity. If valid use value and return
     *  true, else return false.
     */
    bool load();

    /*! Set value
     *  \param newValue new value to set
     *
     *  If newValue is valid, set it and return true, else return false.
     */
    bool set(String newValue);

    String typecode() { return String("i"); }

};

