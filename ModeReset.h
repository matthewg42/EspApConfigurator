#pragma once

#include "NamedMode.h"

//! \brief Resets WiFi device - clears state like network details and other settings
class ModeReset_ : public NamedMode {
public:
    ModeReset_();
    void modeStart();
    void modeUpdate() {;}

};

extern ModeReset_ ModeReset;

