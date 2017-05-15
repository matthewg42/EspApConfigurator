#include <MutilaDebug.h>
#include "WifiClientMode.h"
#include "WifiClientConnecting.h"
#include "WifiClientConnected.h"

WifiClientMode_ WifiClientMode;

WifiClientMode_::WifiClientMode_() 
{
}

void WifiClientMode_::begin()
{
    DBLN(F("WifiClientMode::begin()"));
}

void WifiClientMode_::modeStart()
{
    DBLN(F("WifiClientMode::start()"));
    switchMode(&WifiClientConnecting);
}

void WifiClientMode_::modeStop()
{
    DBLN(F("WifiClientMode::stop()"));
}

void WifiClientMode_::modeUpdate()
{
    if (mode) { 
        mode->update();
        if (mode->isFinished()) {
            if (mode == &WifiClientConnecting) {
                DBLN(F("WifiClientConnecting done -> WifiClientConnected"));
                switchMode(&WifiClientConnected);
            } else if (mode == &WifiClientConnected) {
                DBLN(F("WifiClientConnected done -> WifiClientConnecting"));
                switchMode(&WifiClientConnecting);
            }
        }
    }
}


