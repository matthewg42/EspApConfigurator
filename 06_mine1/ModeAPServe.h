#pragma once

#include <ESP8266WiFi.h>
#include "NamedMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeAPServe_ : public NamedMode {
public:
    ModeAPServe_();
    void modeStart();
    void modeUpdate();

protected:
    IPAddress apIP;
    IPAddress netMsk;

};

extern ModeAPServe_ ModeAPServe;

