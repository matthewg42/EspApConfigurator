#pragma once

#include <Mode.h>

//! \brief Connecting to WiFi
//! This mode waits until WiFi connected, and then terminates
class NormalConnected_ : public Mode {
public:
    NormalConnected_();
    void modeStart();
    void modeUpdate();
    bool isFinished();

private:
    unsigned long _nextWifiCheck;
    bool _isConnected;

};

extern NormalConnected_ NormalConnected;

