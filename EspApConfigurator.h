#pragma once

#include "ParentMode.h"

/*! \brief Access Point Configurator 
 *
 * A different approach to WiFiManager, but doing basically the same thing
 */

class EspApConfigurator_ : public ParentMode {
public:
    EspApConfigurator_();
    void begin();
    void modeStart();
    void modeEnd();
    void modeUpdate();

};

extern EspApConfigurator_ EspApConfigurator;

