#pragma once

#include <Mode.h>
#include <ESP8266WebServer.h>

//! \brief Connecting to WiFi
//! This mode waits until WiFi connected, and then terminates
class WifiClientConnected_ : public Mode {
public:
    WifiClientConnected_();
    void begin();
    void modeStart();
    void modeUpdate();
    bool isFinished();

private:
    unsigned long _nextWifiCheck;
    bool _isConnected;

    void wifiCheck();

};

extern WifiClientConnected_ WifiClientConnected;

