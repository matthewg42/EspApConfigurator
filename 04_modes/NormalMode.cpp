#include <MutilaDebug.h>
#include "NormalMode.h"
#include "NormalConnecting.h"
#include "DummyMode.h"

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
        if (mode->isFinished() && mode == &NormalConnecting) {
            DBLN(F("NormalConnecting done -> DummyMode"));
            switchMode(&DummyMode);
        }
    }
}


