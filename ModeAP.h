#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "NamedMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeAP_ : public NamedMode {
public:
    ModeAP_();
    void modeStart();
    void modeEnd();
    void modeUpdate();
    //! begin asynchronous scanning for wifi networks
    void startScan();
    void finish();
    bool isFinished();

protected:
    DNSServer* pDnsServer;
    bool scanning;
    unsigned long lastScan;
    bool finishFlag;

};

extern ModeAP_ ModeAP;

