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
    EspApConfigurator_(uint8_t maxSettings=16);

    //! Destructor
    ~EspApConfigurator_();

    //! Initialization - call from setup()
    //!
    //! \param apButtonPin pin for button (pull to ground) to active AP, -1 means "none"
    //! \param heartbeatPin pin for heartbeat LED
    //! \param heartbeatInv invert heartbeat logic (e.g. for D0/D4 on-board on NodeMCU)
    //! \param interfaceMode which type of web interface we want to use
    void begin(int8_t apButtonPin=-1, uint8_t heartbeatPin=D0, bool heartbeatInv=true, HttpServer_::Mode interfaceMode=HttpServer_::SinglePage, bool apButtonPullup=true);

    //! This gets called (indirectly) from begin()
    void modeStart();

    //! This gets called if the mode is terminated using stop()
    void modeEnd();

    //! This should be called frequently, typically from loop()
    void modeUpdate();

    //! Go into AP mode (for projects which don't use the button)
    //! \param accessPoint when true set ModeAP, else set ModeWifiClient
    void setApMode(bool accessPoint=true);

    //! Find out if we are in AP mode
    bool inApMode();

    //! Find out if we are connected to wireless network
    bool isConnected();

    //! Get a pointer to the heartbeat object
    Heartbeat* heartbeat();

    //! Should be called before AP mode is entered
    //! \param ssid the SSID to be used when in access point mode
    //! \param pass the passphrase to be used when in access point mode
    // Setting pass to NULL will cause the access point to run in open Wifi mode.
    void setApDetails(const char* ssid, const char* pass);

protected:
    DebouncedButton* _apButton;
    Heartbeat* _heartbeat;

};

extern EspApConfigurator_ EspApConfigurator;

