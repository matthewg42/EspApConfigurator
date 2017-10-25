#pragma once

#include <Mode.h>

//! \brief Resets WiFi device - clears state like network details and other settings
class ModeReset_ : public Mode {
public:
    ModeReset_();
    void modeStart();
    void modeUpdate() {;}

};

extern ModeReset_ ModeReset;

