#include "EspApConfigurator.h"
#include "ModeAP.h"
#include "ModeWifiClient.h"
#include "ModeReset.h"
#include "HttpServer.h"
#include "Config.h"

EspApConfigurator_ EspApConfigurator;

EspApConfigurator_::EspApConfigurator_() :
    PersistentSettingManager(NUMBER_OF_SETTINGS)
{
    _apButton = NULL;
    _heartbeat = NULL;
}

EspApConfigurator_::~EspApConfigurator_()
{
    // Delete any dynamically allocated stuff
    // Will probably never be called because we globally allocate a
    // single instance of EspApConfigurator_ and never delete it,
    // but I fell like this is good practise in case I change how 
    // this stuff works
    if (_apButton) {
        delete _apButton;
        _apButton = NULL;
    }
    if (_heartbeat) {
        delete _heartbeat;
        _heartbeat = NULL;
    }
}

void EspApConfigurator_::begin(uint8_t apButtonPin, uint8_t heartbeatPin, bool heartbeatInv, HttpServer_::Mode interfaceMode)
{
    PersistentSettingManager::begin();
    _apButton = new DebouncedButton(apButtonPin);
    _apButton->begin();
    _heartbeat = new Heartbeat(heartbeatPin, heartbeatInv);
    _heartbeat->begin();
    ModeAP.begin();
    ModeReset.begin();
    ModeWifiClient.begin();
    HttpServer.begin(interfaceMode);
    start();
}

void EspApConfigurator_::modeStart()
{
    DBLN(F("EspApConfigurator::modeStart"));
    switchMode(&ModeWifiClient);
}

void EspApConfigurator_::modeEnd()
{
    DBLN(F("EspApConfigurator::modeEnd"));
    // Don't expect this to run, but heh
}

void EspApConfigurator_::modeUpdate()
{

    _heartbeat->update();
    _apButton->update();

    if (pMode != NULL) {
        pMode->update();
    }

    if (_apButton->held(3000)) {
        switchMode(&ModeReset);
    }

    if (inApMode()) {
        if (pMode->isFinished()) {
            switchMode(&ModeWifiClient);
        }
    } else if (_apButton->tapped()) {
        switchMode(&ModeAP);
    }

    // Let the ESP's Wifi components have a go at the CPU.
    yield();
}

bool EspApConfigurator_::inApMode()
{
    return (pMode == &ModeAP);
}

bool EspApConfigurator_::isConnected()
{
    // Note: When/if we implement static IP mode, this may need to change
    // https://github.com/esp8266/Arduino/issues/119
    return !inApMode() && WiFi.status() == WL_CONNECTED;
}

Heartbeat* EspApConfigurator_::heartbeat() 
{
    return _heartbeat;
}

void EspApConfigurator_::setApDetails(const char* ssid, const char* pass)
{
    ModeAP.setApDetails(ssid, pass);
    if (inApMode()) {
        switchMode(&ModeWifiClient);
        switchMode(&ModeAP);
    }
}

