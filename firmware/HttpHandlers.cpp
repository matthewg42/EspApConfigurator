#include <MutilaDebug.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HttpServer.h"
#include "HttpHandlers.h"

// Taken from MIT-licensed WiFiManager code
int rssiToQuality(int RSSI) {
    int quality = 0;

    if (RSSI <= -100) {
        quality = 0;
    } else if (RSSI >= -50) {
        quality = 100;
    } else {
        quality = 2 * (RSSI + 100);
    }
    return quality;
}

void handleNotFound() {
    DBLN(F("handleNotFound"));
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += pHttpServer->uri();
    message += "\nMethod: ";
    message += ( pHttpServer->method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += pHttpServer->args();
    message += "\n";

    for ( uint8_t i = 0; i < pHttpServer->args(); i++ ) {
        message += " " + pHttpServer->argName ( i ) + ": " + pHttpServer->arg ( i ) + "\n";
    }
    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->send ( 404, "text/plain", message );
}

void handleRoot()
{
    DBLN(F("handleRoot"));
    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "EspApConfigurator");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page += F("<h1>EspApConfigurator</h1>");
    page += FPSTR(HTTP_PORTAL_OPTIONS);
    page += FPSTR(HTTP_END);

    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->send(200, "text/html", page);
    // pHttpServer->client().stop();
}

void handleSettingsPage()
{
    DBLN(F("handleSettingsPage"));

}

void handleWifiPage()
{
    DBLN(F("handleWifiPage"));

    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Config WiFi");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);

    // TODO: sort by signal strength
    //
    int8_t netCount = WiFi.scanComplete();
    for (int8_t i=0; i<netCount; i++) {
        // "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>"
        String item = FPSTR(HTTP_ITEM);
        item.replace("{v}", WiFi.SSID(i));
        item.replace("{r}", String(rssiToQuality(WiFi.RSSI(i))));
        if (WiFi.encryptionType(i) != ENC_TYPE_NONE) {
            item.replace("{i}", "l");
        } else {
            item.replace("{i}", "");
        }
        page += item;
    }
    page += F("<br/>");
    page += FPSTR(HTTP_FORM_START);
    page += FPSTR(HTTP_FORM_END);
    page += F("<br/>");
    page += FPSTR(HTTP_END);

    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->send(200, "text/html", page);
    // pHttpServer->client().stop();
}

