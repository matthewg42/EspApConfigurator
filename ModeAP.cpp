#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include <Millis.h>
#include "ModeAP.h"
#include "MutilaDebug.h"
#include "HeartBeat.h"
#include "HttpServer.h"
#include "HttpHandlers.h"
#include "ModeWifiClient.h"
#include "Config.h"

ModeAP_ ModeAP;

ModeAP_::ModeAP_() :
    NamedMode("ModeAP")
{
    pDnsServer = NULL;
    pHttpServer = NULL;
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
    WiFi.softAP(AP_NAME, AP_PASS);

    // Wait a moment for ESP state to change
    delay(100);
    DB(F("AP IP address="));
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
    pHttpServer->onNotFound(handleNotFound);
    pHttpServer->on("/", handleRoot);
    pHttpServer->on("/save", handleWifiSave);
    pHttpServer->on("/wifi", handleWifi);
    pHttpServer->on("/r", handleRescan);
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

}

void ModeAP_::modeUpdate()
{
    // Handle incoming connections
    pDnsServer->processNextRequest();
    pHttpServer->handleClient();

    if (scanning) {
        int8_t netCount = WiFi.scanComplete();
        if (netCount >= 0) {
            // show that scan has completed, and now AP mode is listening
            HeartBeat.setCustomMode(900, 100);
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
    HeartBeat.setMode(Heartbeat::Quicker);
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

