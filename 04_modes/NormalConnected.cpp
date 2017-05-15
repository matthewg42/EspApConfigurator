#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MutilaDebug.h"
#include "NormalConnected.h"
#include "HeartBeat.h"
#include "Config.h"

NormalConnected_ NormalConnected;

NormalConnected_::NormalConnected_()
{
}

void NormalConnected_::modeStart()
{
    DBLN(F("NormalConnected::modeStart"));
    HeartBeat.setMode(Heartbeat::Normal);
    _isConnected = true;
    _nextWifiCheck = Millis() + WIFI_CHECK_MS;
}

void NormalConnected_::modeUpdate()
{
    if (Millis() > _nextWifiCheck) {
        _nextWifiCheck = Millis();
        if (WiFi.status() != WL_CONNECTED) {
            DBLN(F("WiFi dropped"));
            _isConnected = false;
            return;
        }
    }
}

bool NormalConnected_::isFinished()
{
    return !_isConnected;
}

