#pragma once

#include <Mode.h>

//! \brief Connecting to WiFi
//! This mode waits until WiFi connected, and then terminates
class NormalConnecting_ : public Mode {
public:
    NormalConnecting_();
    void modeStart();
    void modeUpdate();
    bool isFinished();

private:
    bool _isConnected;

};

extern NormalConnecting_ NormalConnecting;

