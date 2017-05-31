#pragma once

#include <ESP8266WiFi.h>
#include <DNSServer.h>
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
    DNSServer dnsServer;

};

extern ModeAPServe_ ModeAPServe;

