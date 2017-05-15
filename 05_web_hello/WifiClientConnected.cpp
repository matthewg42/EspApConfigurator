#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MutilaDebug.h"
#include "WifiClientConnected.h"
#include "HeartBeat.h"
#include "MyAPI.h"
#include "Config.h"

WifiClientConnected_ WifiClientConnected;

WifiClientConnected_::WifiClientConnected_()
{
}

void WifiClientConnected_::begin()
{
    DBLN(F("WifiClientConnected::begin"));
}

void WifiClientConnected_::modeStart()
{
    DBLN(F("WifiClientConnected::modeStart"));
    MyAPI_begin();
    HeartBeat.setMode(Heartbeat::Normal);
    _isConnected = true;
    _nextWifiCheck = Millis() + WIFI_CHECK_MS;
}

void WifiClientConnected_::modeUpdate()
{
    wifiCheck();
    MyAPI.handleClient();
}

bool WifiClientConnected_::isFinished()
{
    return !_isConnected;
}

void WifiClientConnected_::wifiCheck()
{
    if (Millis() > _nextWifiCheck) {
        _nextWifiCheck = Millis();
        if (WiFi.status() != WL_CONNECTED) {
            DBLN(F("WiFi dropped"));
            _isConnected = false;
        }
    }
}

