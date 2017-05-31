#include "ParentMode.h"

ParentMode::ParentMode(const char* name) :
    NamedMode(name)
{
    pMode = NULL;
}

void ParentMode::switchMode(NamedMode* newMode)
{
    if (pMode != NULL) {
        pMode->stop();
    }
    pMode = newMode;
    pMode->start();
    _pname = _name + ":" + pMode->name();
}

void ParentMode::modeUpdate()
{
    if (pMode != NULL) {
        pMode->update();
    }
}

const char* ParentMode::name()
{
    return _pname.c_str();
}
