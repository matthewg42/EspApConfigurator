#include "EspApConfigurator.h"
#include "ModeAP.h"
#include "ModeWifiClient.h"
#include "ModeReset.h"
#include "HttpServer.h"
#include "APButton.h"
#include "HeartBeat.h"
#include "Config.h"

EspApConfigurator_ EspApConfigurator;

EspApConfigurator_::EspApConfigurator_(HttpServer_::Mode mode) :
    PersistentSettingManager(NUMBER_OF_SETTINGS),
    _mode(mode)
{
}

void EspApConfigurator_::begin()
{
    PersistentSettingManager::begin();
    APButton.begin();
    HeartBeat.begin();
    ModeAP.begin();
    ModeReset.begin();
    ModeWifiClient.begin();
    HttpServer.begin(_mode);
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
    HeartBeat.update();
    APButton.update();

    if (pMode != NULL) {
        pMode->update();
    }

    if (APButton.held(3000)) {
        switchMode(&ModeReset);
    }

    if (inApMode()) {
        if (pMode->isFinished()) {
            switchMode(&ModeWifiClient);
        }
    } else if (APButton.tapped()) {
        switchMode(&ModeAP);
    }
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


