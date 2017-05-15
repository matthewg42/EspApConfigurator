#include <Arduino.h>
#include <MutilaDebug.h>
#include "DummyMode.h"
#include "MutilaDebug.h"

DummyMode_ DummyMode;

DummyMode_::DummyMode_() 
{
    setUpdatePeriod(500);
}

void DummyMode_::modeStart()
{
    DBLN(F("DummyMode::modeStart()"));
}

void DummyMode_::modeUpdate()
{
    DBLN(F("DummyMode::modeUpdate()"));
}

