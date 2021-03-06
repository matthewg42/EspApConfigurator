#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PersistentSettingAtom.h"

bool basicBoolValidator(String s);

/*! \brief Class for storing boolean values to EEPROM
 *
 */
class PersistentSettingBool : public PersistentSettingAtom<bool> {
public:
    /*! Constructor
     * \param eepromAddress start address of this setting
     * \param defaultValue value to take before loading
     * \param validatorFunction function to validate values
     */
    PersistentSettingBool(uint16_t eepromAddress, bool defaultValue, validatorFunction validator=&basicBoolValidator);

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
    bool set(String newValue) override;
    String typecode() override { return String("b"); }

};

