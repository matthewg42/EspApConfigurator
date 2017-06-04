#pragma once

#include "ParentMode.h"
#include "PersistentSettingManager.h"

/*! \brief Access Point Configurator 
 *
 * A different approach to WiFiManager, but doing basically the same thing
 */

class EspApConfigurator_ : public ParentMode, public PersistentSettingManager {
public:
    EspApConfigurator_();
    void begin();
    void modeStart();
    void modeEnd();
    void modeUpdate();
    bool inApMode();

};

extern EspApConfigurator_ EspApConfigurator;

