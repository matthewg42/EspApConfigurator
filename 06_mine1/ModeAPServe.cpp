#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeAPServe.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"
#include "Config.h"

ModeAPServe_ ModeAPServe;

ModeAPServe_::ModeAPServe_() :
    NamedMode("ModeAPServe"),
    apIP(192, 168, 6, 1),
    netMsk(255, 255, 255, 0)
{
    setUpdatePeriod(100);
}

void ModeAPServe_::modeStart()
{
    DBLN(F("S:ModeAPServe::modeStart()"));
    pDnsServer = new DNSServer();
    HeartBeat.setMode(Heartbeat::Quick);
    // Make sure the WiFi is turned on
    WiFi.forceSleepWake();
    WiFi.mode(WIFI_STA);
    delay(500);
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(AP_NAME, AP_PASS);
    delay(300);
    DB(" AP IP address: ");
    DBLN(WiFi.softAPIP());
    pDnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    pDnsServer->start(53, "*", WiFi.softAPIP());
    DBLN(F("E:ModeAPServe::modeStart()"));
}

void ModeAPServe_::modeEnd()
{
    if (pDnsServer) {
        delete pDnsServer;
        pDnsServer = NULL;
    }
}

void ModeAPServe_::modeUpdate()
{
    DBLN(F("S:ModeAPServe::modeUpdate()"));
    if (pDnsServer) {
        //pDnsServer->processNextRequest();
    }
    DBLN(F("E:ModeAPServe::modeUpdate()"));
}

