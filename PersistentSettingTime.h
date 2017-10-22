#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PersistentSettingAtom.h"

long timeStrToSeconds(String s);
bool basicTimeValidator(String s);
String secondsToTimeStr(long seconds, bool showSeconds=true);

/*! \brief Class for storing time values to EEPROM
 */
class PersistentSettingTime : public PersistentSettingAtom<long> {
public:
    /*! Constructor
     * \param eepromAddress start address of this setting
     * \param defaultValue value to take before loading
     * \param displaySeconds if true, get() will return seconds in value, else only to minute precision
     * \param validatorFunction function to validate values
     */
    PersistentSettingTime(uint16_t eepromAddress, String defaultValue, bool displaySeconds=false, validatorFunction validator=basicTimeValidator);

    /*! Load from EEPROM 
     *  Loads value from EEPROM, and checks for validity. If valid use value and return
     *  true, else return false.
     */
    bool load();

    /*! Get value
     *
     *  If Get String representatin of the value
     */
    String get();

    /*! Set value
     *  \param newValue new value to set
     *
     *  If newValue is valid, set it and return true, else return false.
     */
    bool set(String newValue);

    String typecode() { return String("t"); }

protected:
    bool _displaySeconds;

};

