#include <Arduino.h>
#include <MutilaDebug.h>
#include "ModeDummy.h"
#include "MutilaDebug.h"

ModeDummy_ ModeDummy;

ModeDummy_::ModeDummy_() :
    NamedMode("ModeDummy")
{
    setUpdatePeriod(500);
}

void ModeDummy_::modeStart()
{
    DBLN(F("ModeDummy::modeStart()"));
}

void ModeDummy_::modeUpdate()
{
    DBLN(F("ModeDummy::modeUpdate()"));
}

