#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PersistentSettingAtom.h"

/*! \brief Class for storing long int values to EEPROM
 */
class PersistentSettingFloat : public PersistentSettingAtom<float> {
public:
    /*! Constructor
     * \param eepromAddress start address of this setting
     * \param defaultValue value to take before loading
     * \param validatorFunction function to validate values
     */
    PersistentSettingFloat(uint16_t eepromAddress, float defaultValue, validatorFunction validator=NULL);

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

};

