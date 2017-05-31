#pragma once

#include "NamedMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeDummy_ : public NamedMode {
public:
    ModeDummy_();
    void modeStart();
    void modeUpdate();

};

extern ModeDummy_ ModeDummy;

