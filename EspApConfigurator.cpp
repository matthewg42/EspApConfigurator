#include "EspApConfigurator.h"
#include "ModeAP.h"
#include "ModeWifiClient.h"
#include "ModeReset.h"
#include "APButton.h"
#include "HeartBeat.h"
#include "Config.h"

EspApConfigurator_ EspApConfigurator;

EspApConfigurator_::EspApConfigurator_() :
    ParentMode("EspApConfigurator"),
    PersistentSettingManager(NUMBER_OF_SETTINGS)
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
}

void EspApConfigurator_::modeStart()
{
    DBLN(F("EspApConfigurator::modeStart"));
    switchMode(&ModeWifiClient);
}

void EspApConfigurator_::modeEnd()
{
    DBLN(F("EspApConfigurator::modeEnd"));
    switchMode(&ModeWifiClient);
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

