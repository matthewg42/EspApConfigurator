#include "EspApConfigurator.h"
#include "ModeAP.h"
#include "ModeWifiClient.h"
#include "ModeReset.h"
#include "APButton.h"
#include "HeartBeat.h"

EspApConfigurator_ EspApConfigurator;

EspApConfigurator_::EspApConfigurator_() :
    ParentMode("EspApConfigurator")
{
}

void EspApConfigurator_::begin()
{
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

    if (pMode == &ModeAP) {
        if (pMode->isFinished()) {
            switchMode(&ModeWifiClient);
        }
    } else if (APButton.tapped()) {
        switchMode(&ModeAP);
    }
}

