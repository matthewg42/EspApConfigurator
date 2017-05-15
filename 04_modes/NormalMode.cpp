#include <MutilaDebug.h>
#include "NormalMode.h"
#include <DummyMode.h>

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
    switchMode(&DummyMode);
}

void NormalMode_::modeStop()
{
    DBLN(F("NormalMode::stop()"));
}

void NormalMode_::modeUpdate()
{
    if (mode) mode->update();
}


