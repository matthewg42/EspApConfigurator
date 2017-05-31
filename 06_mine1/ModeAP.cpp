#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include "ModeAP.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"
#include "HttpServer.h"
#include "HttpHandlers.h"
#include "ModeWifiClient.h"
#include "Config.h"

ModeAP_ ModeAP;

ModeAP_::ModeAP_() :
    NamedMode("ModeAP"),
    apIP(192, 168, 6, 1),
    netMsk(255, 255, 255, 0)
{
    pDnsServer = NULL;
    pHttpServer = NULL;
}

void ModeAP_::modeStart()
{
    DBLN(F("S:ModeAP::modeStart()"));
    
    // Change heartbeat to indicate AP mode
    HeartBeat.setCustomMode(900, 100);

    // Make sure the WiFi is turned on
    WiFi.forceSleepWake();

    // This is required for AP mode to work properly with my setup
    // (NodeMCU)
    WiFi.mode(WIFI_STA);

    // Put into AP mode, with specified IP/netmask
    WiFi.softAPConfig(apIP, apIP, netMsk);

    // Set AP SSID and passphrase
    WiFi.softAP(AP_NAME, AP_PASS);

    DB("AP IP address: ");
    DBLN(WiFi.softAPIP());

    // Set up DNS resolver for all names going to local IP
    if (!pDnsServer) {
        pDnsServer = new DNSServer();
    }
    pDnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    pDnsServer->start(53, "*", WiFi.softAPIP());

    // Set up routes for web server
    if (!pHttpServer) {
        pHttpServer = new ESP8266WebServer(80);
    }
    pHttpServer->on("/", handleRoot);
    pHttpServer->onNotFound(handleNotFound);
    pHttpServer->begin();

    DBLN(F("E:ModeAP::modeStart()"));
}

void ModeAP_::modeEnd()
{
    DBLN("ModeAP::modeEnd");
    // destroy DNS to save memory when not in AP mode
    if (pDnsServer) {
        delete pDnsServer;
        pDnsServer = NULL;
    }

    // destroy HTTP server to save memory when not in AP mode
    if (pHttpServer) {
        delete pHttpServer;
        pHttpServer = NULL;
    }

    // TODO: remove (this code for testing)
    ModeWifiClient.setWifiLogin("wirescubedextended", "16leslierd");
}

void ModeAP_::modeUpdate()
{
    // Handle incoming connections
    pDnsServer->processNextRequest();
    pHttpServer->handleClient();
}

