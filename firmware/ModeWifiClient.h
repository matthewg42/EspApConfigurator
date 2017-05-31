#pragma once

#include <Arduino.h>
#include "NamedMode.h"

//! \brief Normal operation as WiFi client
class ModeWifiClient_ : public NamedMode {
public:
    ModeWifiClient_();
    void modeStart();
    void modeUpdate();
    void setWifiLogin(const char* ssid, const char* passphrase);
    void connect(bool useCredentials=true);

protected:
    String _ssid;
    String _passphrase;
    int _prevStatus;

};

extern ModeWifiClient_ ModeWifiClient;

