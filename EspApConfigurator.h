#pragma once

#include <stdint.h>
#include <ParentMode.h>
#include <DebouncedButton.h>
#include <Heartbeat.h>
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

    //! Destructor
    ~EspApConfigurator_();

    //! Initialization - call from setup()
    //!
    //! \param apButtonPin pin for button (pull to ground) to active AP
    //! \param heartbeatPin pin for heartbeat LED
    //! \param heartbeatInv invert heartbeat logic (e.g. for D0/D4 on-board on NodeMCU)
    //! \param interfaceMode which type of web interface we want to use
    void begin(uint8_t apButtonPin, uint8_t heartbeatPin=D0, bool heartbeatInv=true, HttpServer_::Mode interfaceMode=HttpServer_::SinglePage);

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

    //! Get a pointer to the heartbeat object
    Heartbeat* heartbeat();

protected:
    DebouncedButton* _apButton;
    Heartbeat* _heartbeat;

};

extern EspApConfigurator_ EspApConfigurator;

