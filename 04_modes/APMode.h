#pragma once

#include <Mode.h>

//! \brief Switch to APMode to set up WiFi
class APMode_ : public Mode {
public:
    APMode_();
    void modeStart();
    void modeUpdate();

};

extern APMode_ APMode;

