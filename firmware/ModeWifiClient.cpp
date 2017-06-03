#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeWifiClient.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"

ModeWifiClient_ ModeWifiClient;

ModeWifiClient_::ModeWifiClient_() :
    NamedMode("ModeWifiClient"),
    _prevStatus(254) // not a real status, so we get debug on first update
{
    setUpdatePeriod(100);
}

void ModeWifiClient_::modeStart()
{
    DBLN(F("ModeWifiClient::modeStart()"));
    HeartBeat.setMode(Heartbeat::Quick);
    WiFi.forceSleepWake(); 
    connect(_ssid!="");
}

void ModeWifiClient_::setWifiLogin(const char* ssid, const char* passphrase)
{
    DB(F("ModeWifiClient::setWifiLogin ssid="));
    DB(ssid);
    DB(F(" passphrase "));
    DBLN(passphrase==NULL ? F("not set") : F("********"));
    _ssid = ssid;
    _passphrase = passphrase;
}

void ModeWifiClient_::setHostname(const char* hostname)
{
    DB(F("ModeWifiClient::setHostname "));
    _hostname = hostname;
    DBLN(_hostname);
}

void ModeWifiClient_::connect(bool useCredentials)
{
    DB(F("ModeWifiClient_::connect useCredentials="));
    DBLN(useCredentials);
    WiFi.disconnect();
    if (useCredentials) {
        WiFi.begin(_ssid.c_str(), _passphrase.c_str());
        WiFi.hostname(_hostname.c_str());
    } else { 
        // Let the ESP8266 use the last sucessful credentials
        WiFi.begin();
    }
}

void ModeWifiClient_::modeUpdate()
{
    int status = WiFi.status();
    if (status != _prevStatus) {
        DB(F("Wifi: "));
        switch (status) {
        case WL_CONNECTED:
            DB(F("connected ip="));
            HeartBeat.setMode(Heartbeat::Slower);
            DBLN(WiFi.localIP());
            break;
        case WL_IDLE_STATUS:
            DBLN(F("idle"));
            HeartBeat.setMode(Heartbeat::Normal);
            break;
        case WL_CONNECT_FAILED:
            DBLN(F("connect failed"));
            HeartBeat.setMode(Heartbeat::Quick);
            break;
        case WL_CONNECTION_LOST:
            DBLN(F("connect lost"));
            HeartBeat.setMode(Heartbeat::Quick);
            break;
        case WL_DISCONNECTED:
            DBLN(F("connect disconnected"));
            HeartBeat.setMode(Heartbeat::Normal);
            break;
        default:
            DBLN(status);
            HeartBeat.setMode(Heartbeat::Quick);
            break;
        }
        _prevStatus = status;
    }
}

