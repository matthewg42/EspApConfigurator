#pragma once

#include <ParentMode.h>
#include "PersistentSettingManager.h"
#include "HttpServer.h"

/*! \brief Access Point Configurator 
 *
 * A different approach to WiFiManager, but doing basically the same thing
 */

class EspApConfigurator_ : public ParentMode, public PersistentSettingManager {
public:
    //! Constructor
    EspApConfigurator_();

    //! Initialization - call from setup()
    //!
    //! \param interfaceMode which type of web interface we want to use
    void begin(HttpServer_::Mode interfaceMode=HttpServer_::SinglePage);

    //! This gets called (indirectly) from begin()
    void modeStart();

    //! This gets called if the mode is terminated using stop()
    void modeEnd();

    //! This should be called frequently, typically from loop()
    void modeUpdate();

    //! Find out if we are in AP mode
    bool inApMode();

    //! Find out if we are connected to wireless network
    bool isConnected();

};

extern EspApConfigurator_ EspApConfigurator;

