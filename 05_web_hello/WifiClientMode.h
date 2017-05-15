#pragma once

#include "ParentMode.h"

//! \brief WifiClientMode for normal operation (connected to some WiFi)
//! In WifiClientMode, we use WiFi credentials from EEPROM and connect
//! to a WiFi network. The HeartBeat is set according to connection
//! status - i.e. quick flashing when not connected, normal flashing
//! otherwise.
class WifiClientMode_ : public ParentMode {
public:
    WifiClientMode_();
    void begin();
    void modeStart();
    void modeStop();
    void modeUpdate();

};

extern WifiClientMode_ WifiClientMode;

