#pragma once

#include <Mode.h>

//! \brief Mode which does nothing (except debug out)
class ModeDummy_ : public Mode {
public:
    ModeDummy_();
    void modeStart();
    void modeUpdate();

};

extern ModeDummy_ ModeDummy;

