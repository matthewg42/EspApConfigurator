#include <Arduino.h>
#include <MutilaDebug.h>
#include "ModeAPBase.h"
#include "MutilaDebug.h"
#include "ModeAPServe.h"

ModeAPBase_ ModeAPBase;

ModeAPBase_::ModeAPBase_() :
    ParentMode("ModeAPBase")
{
}

void ModeAPBase_::begin()
{
    DBLN(F("ModeAPBase::begin()"));
    ModeAPServe.begin();
}

void ModeAPBase_::modeStart()
{
    DBLN(F("ModeAPBase::modeStart()"));
    switchMode(&ModeAPServe);
}

void ModeAPBase_::modeUpdate()
{
    ParentMode::modeUpdate();
}

