#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Mode.h>

//! \brief Mode which does nothing (except debug out)
class ModeAP_ : public Mode {
public:
    ModeAP_();
    void modeStart();
    void modeEnd();
    void modeUpdate();
    //! begin asynchronous scanning for wifi networks
    void startScan();
    void finish();
    bool isFinished();
    void setApDetails(const char* ssid, const char* pass);

protected:
    String _ssid;
    String _pass;
    bool scanning;
    bool wantScan;
    bool finishFlag;

};

extern ModeAP_ ModeAP;

