#include <MutilaDebug.h>
#include "NormalMode.h"
#include "NormalConnecting.h"
#include "NormalConnected.h"

NormalMode_ NormalMode;

NormalMode_::NormalMode_() 
{
}

void NormalMode_::begin()
{
    DBLN(F("NormalMode::begin()"));
}

void NormalMode_::modeStart()
{
    DBLN(F("NormalMode::start()"));
    switchMode(&NormalConnecting);
}

void NormalMode_::modeStop()
{
    DBLN(F("NormalMode::stop()"));
}

void NormalMode_::modeUpdate()
{
    if (mode) { 
        mode->update();
        if (mode->isFinished()) {
            if (mode == &NormalConnecting) {
                DBLN(F("NormalConnecting done -> NormalConnected"));
                switchMode(&NormalConnected);
            } else if (mode == &NormalConnected) {
                DBLN(F("NormalConnected done -> NormalConnecting"));
                switchMode(&NormalConnecting);
            }
        }
    }
}


