#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MutilaDebug.h"
#include "WifiClientConnecting.h"
#include "HeartBeat.h"
#include "Config.h"

WifiClientConnecting_ WifiClientConnecting;

WifiClientConnecting_::WifiClientConnecting_() :
    _isConnected(false)
{
    // only check if we're connected every 500ms
    setUpdatePeriod(WIFI_CHECK_MS);
}

void WifiClientConnecting_::modeStart()
{
    DBLN(F("WifiClientConnecting::modeStart"));
    HeartBeat.setMode(Heartbeat::Quick);
}

void WifiClientConnecting_::modeUpdate()
{
    if (WiFi.status() != WL_CONNECTED) {
        return;
    } else {
        DB(F("WiFi connected, IP="));
        DBLN(WiFi.localIP());
        _isConnected = true;
    }
}

bool WifiClientConnecting_::isFinished()
{
    return _isConnected;
}

