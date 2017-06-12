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
    EspApConfigurator_(HttpServer_::Mode mode=HttpServer_::SinglePage);

    //! Initialization - call from setup()
    void begin();

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

protected:
    HttpServer_::Mode _mode;

};

extern EspApConfigurator_ EspApConfigurator;

