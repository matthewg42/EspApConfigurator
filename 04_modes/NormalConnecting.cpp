#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MutilaDebug.h"
#include "NormalConnecting.h"
#include "HeartBeat.h"

NormalConnecting_ NormalConnecting;

NormalConnecting_::NormalConnecting_() :
    _isConnected(false)
{
    // only check if we're connected every 500ms
    setUpdatePeriod(500);
}

void NormalConnecting_::modeStart()
{
    DBLN(F("NormalConnecting::modeStart"));
    HeartBeat.setMode(Heartbeat::Quick);
}

void NormalConnecting_::modeUpdate()
{
    if (WiFi.status() != WL_CONNECTED) {
        return;
    } else {
        DB(F("WiFi connected, IP="));
        DBLN(WiFi.localIP());
        _isConnected = true;
    }
}

bool NormalConnecting_::isFinished()
{
    return _isConnected;
}

