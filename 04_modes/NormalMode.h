#pragma once

#include "ParentMode.h"

//! \brief NormalMode for normal operation (connected to some WiFi)
//! In NormalMode, we use WiFi credentials from EEPROM and connect
//! to a WiFi network. The HeartBeat is set according to connection
//! status - i.e. quick flashing when not connected, normal flashing
//! otherwise.
class NormalMode_ : public ParentMode {
public:
    NormalMode_();
    void begin();
    void modeStart();
    void modeStop();
    void modeUpdate();

};

extern NormalMode_ NormalMode;

