#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include "ModeAP.h"
#include "MutilaDebug.h"
#include "HttpServer.h"
#include "HttpHandlers.h"
#include "ModeWifiClient.h"
#include "EspApConfigurator.h"
#include "Config.h"

ModeAP_ ModeAP;

ModeAP_::ModeAP_() :
    _ssid(AP_NAME),
    _pass(AP_PASS)
{
}

void ModeAP_::modeStart()
{
    DBLN(F("S:ModeAP::modeStart()"));
    
    // Reset the flag so we keep running
    finishFlag = false;

    // We're not scanning yet, but we'd like to
    scanning = false;
    wantScan = true;

    // Make sure the WiFi is turned on
    WiFi.forceSleepWake();

    // This is required for AP mode to work properly with my setup
    // (NodeMCU)
    WiFi.mode(WIFI_STA);

    // Set AP SSID and passphrase
    DB(F("AP SSID="));
    DB(_ssid);
    DB(' ');
    DBLN(_pass == "" ? F("[open]") : F("[passphrase]"));
    WiFi.softAP(_ssid.c_str(), _pass == "" ? NULL : _pass.c_str());

    // Wait a moment for ESP state to change
    delay(100);
    DB(F("AP IP address="));
    DBLN(WiFi.softAPIP());

    DBLN(F("E:ModeAP::modeStart()"));
}

void ModeAP_::modeEnd()
{
    DBLN("ModeAP::modeEnd");
}

void ModeAP_::modeUpdate()
{
    // Handle incoming connections
    HttpServer.handleClient();

    if (scanning) {
        int8_t netCount = WiFi.scanComplete();
        if (netCount >= 0) {
            // show that scan has completed, and now AP mode is listening
            EspApConfigurator.heartbeat()->setCustomMode(900, 100);
            scanning = false;
            DB(F("Scan complete, found "));
            DB(netCount);
            DBLN(F(" net(s)"));
        }
    } else if (wantScan) {
        startScan();
    }
}

void ModeAP_::startScan()
{
    if (scanning) {
        DBLN(F("[still scanning]"));
        return;
    }
    EspApConfigurator.heartbeat()->setMode(Heartbeat::Quicker);
    wantScan = false;
    scanning = true;
    WiFi.scanNetworks(true);
}

void ModeAP_::finish()
{
    finishFlag = true;
}

bool ModeAP_::isFinished()
{
    return finishFlag;
}

void ModeAP_::setApDetails(const char* ssid, const char* pass)
{
    _ssid = ssid;
    if (pass == NULL) {
        _pass = "";
    } else {
        _pass = pass;
    }
}

