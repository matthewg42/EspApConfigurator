#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "NamedMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeAPServe_ : public NamedMode {
public:
    ModeAPServe_();
    void modeStart();
    void modeEnd();
    void modeUpdate();

protected:
    IPAddress apIP;
    IPAddress netMsk;
    DNSServer* pDnsServer;

};

extern ModeAPServe_ ModeAPServe;

