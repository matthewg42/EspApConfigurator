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
    HeartBeat.setMode(Heartbeat::Normal);
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


void ModeWifiClient_::connect(bool useCredentials)
{
    DB(F("ModeWifiClient_::connect uc="));
    DBLN(useCredentials);
    WiFi.disconnect();
    if (useCredentials) {
        WiFi.begin(_ssid.c_str(), _passphrase.c_str());
    } else { 
        // Let the ESP8266 use the last sucessful credentials
        WiFi.begin();
    }
}


void ModeWifiClient_::modeUpdate()
{
    int status = WiFi.status();
    if (status != _prevStatus) {
        DB(F("Wifi stat: "));
        switch (status) {
        case WL_CONNECTED:
            DB(F("connOK, ip="));
            DBLN(WiFi.localIP());
            break;
        case WL_IDLE_STATUS:
            DBLN(F("idle"));
            break;
        case WL_CONNECT_FAILED:
            DBLN(F("connFail"));
            break;
        case WL_CONNECTION_LOST:
            DBLN(F("connLost"));
            break;
        case WL_DISCONNECTED:
            DBLN(F("connDisc"));
            break;
        default:
            DBLN(status);
            break;
        }
        _prevStatus = status;
    }
}

