#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeAPServe.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"
#include "HttpServer.h"
#include "HttpHandlers.h"
#include "Config.h"

ModeAPServe_ ModeAPServe;

ModeAPServe_::ModeAPServe_() :
    NamedMode("ModeAPServe"),
    apIP(192, 168, 6, 1),
    netMsk(255, 255, 255, 0)
{
    pDnsServer = NULL;
    pHttpServer = NULL;
}

void ModeAPServe_::modeStart()
{
    DBLN(F("S:ModeAPServe::modeStart()"));
    if (!pDnsServer) {
        pDnsServer = new DNSServer();
    }
    if (!pHttpServer) {
        pHttpServer = new ESP8266WebServer(80);
    }
    
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

    // Set up routes for web server
    pHttpServer->on("/", handleRoot);
    pHttpServer->onNotFound(handleNotFound);
    pHttpServer->begin();

    DBLN(F("E:ModeAPServe::modeStart()"));
}

void ModeAPServe_::modeEnd()
{
    if (pDnsServer) {
        delete pDnsServer;
        pDnsServer = NULL;
    }

    if (pHttpServer) {
        delete pHttpServer;
        pHttpServer = NULL;
    }
}

void ModeAPServe_::modeUpdate()
{
    pDnsServer->processNextRequest();
    pHttpServer->handleClient();
}

