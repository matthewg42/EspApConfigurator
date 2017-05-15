#include "ParentMode.h"

ParentMode::ParentMode()
{
    mode = NULL;
}

void ParentMode::switchMode(Mode* newMode)
{
    if (mode != NULL) {
        mode->stop();
    }
    mode = newMode;
    mode->start();
}

void ParentMode::modeUpdate()
{
    if (mode != NULL) {
        mode->update();
    }
}

