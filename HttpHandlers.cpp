#include <MutilaDebug.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HttpServer.h"
#include "HttpHandlers.h"
#include "ModeAP.h"
#include "ModeWifiClient.h"
#include "EspApConfigurator.h"

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

bool validateHostname(String h) {
    DB(F("validateHostname "));
    DB(h);
    if (h == "") {
        DBLN(F(" fail:blank"));
        return false;
    } 
    for (uint8_t i=0; i<h.length(); i++) {
        if (! (
                   (h[i] >= 'a' && h[i] <= 'z') 
                || (h[i] >= 'A' && h[i] <= 'Z') 
                || (h[i] >= '0' && h[i] <= '9') 
                || h[i] == '_')) {
            DBLN(F(" fail:char"));
            return false;
        }
    }
    DBLN(F(" OK"));
    return true;
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
    pHttpServer->send (404, "text/plain", message);
    pHttpServer->client().stop();
}

void handleRoot()
{
    DBLN(F("handleRoot"));

    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "ESPApConfigurator");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page += F("<h1>ESPApConfigurator</h1><h3>Networks</h3>");

    // TODO: sort by signal strength
    int8_t netCount = WiFi.scanComplete();
    if (netCount>0) {
        for (int8_t i=0; i<netCount; i++) {
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
    } else {
        page += F("<p>[no networks found]</p>");
    }
    page += F("<br/>");
    String form = FPSTR(HTTP_FORM_START);
    form.replace("{h}", WiFi.hostname());
    page += form;
    if (EspApConfigurator.count()>0) { F("<h3>ProjectSettings</h3><p>"); }
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
        form = FPSTR(HTTP_FORM_PARAM);
        form.replace("{i}", String('p') + String((char)('a'+i)));
        form.replace("{p}", EspApConfigurator[i].id);
        form.replace("{l}", String(EspApConfigurator[i].setting->formLength()));
        form.replace("{v}", EspApConfigurator[i].setting->get());
        page += form;
    }
    if (EspApConfigurator.count()>0) { F("</p>"); }
    page += FPSTR(HTTP_FORM_END);
    page += F("<br/>");
    page += FPSTR(HTTP_END);

    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    pHttpServer->send(200, "text/html", page);
    pHttpServer->client().stop();
}

void handleWifiSavePage() {
    DBLN(F("handleWifiSavePage"));
    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Saving Settings");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);

    String ssid;
    String pass;
    String host;
    bool ok = true;
    String reason = "";

    if (pHttpServer->method() != HTTP_POST) {
        ok = false;
        reason += F("not a POST<br/>");
    } else {
        // we have a POST, good, now populate the parameters
        // iterate over pHttpServer->args
        ssid = pHttpServer->arg("s");
        pass = pHttpServer->arg("p");
        host = pHttpServer->arg("h");
        if (ssid == "") {
            ok = false;
            reason = F("no ssid specified<br/>");
        } 
        if (!validateHostname(host)) {
            ok = false;
            reason += F("bad hostname \"");
            reason += host;
            reason += '\"<br/>';
        }

        // Handle custom settings...
        for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
            String id = String('p') + String((char)('a'+i));
            String value = pHttpServer->arg(id.c_str());
            if (!EspApConfigurator[i].setting->set(value)) {
                ok = false;
                reason += F("Setting \"");
                reason += EspApConfigurator[i].id;
                reason += F(" \" invalid value \"");
                reason += value;
                reason += "\"<br/>";
            } else if (!EspApConfigurator[i].setting->save()) {
                ok = false;
                reason += F("Setting \"");
                reason += EspApConfigurator[i].id;
                reason += F(" \" save failed<br/>");
            }
        }
    }

    // Evaluate our status and display message accordingly
    if (!ok) {
        page += F("<h1>Error</h1>");       
        page += reason;       
    } else {
        String saved = FPSTR(HTTP_SAVED);
        saved.replace("{h}", host);
        page += saved;
    }

    page += FPSTR(HTTP_END);

    pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    pHttpServer->sendHeader("Pragma", "no-cache");
    pHttpServer->sendHeader("Expires", "-1");
    if (ok) {
        pHttpServer->send(200, "text/html", page);
        pHttpServer->client().stop();
        ModeWifiClient.setWifiLogin(ssid.c_str(), pass=="" ? NULL : pass.c_str());
        ModeWifiClient.setHostname(host.c_str());
        ModeAP.finish();
    } else {
        pHttpServer->send(400, "text/html", page);
        pHttpServer->client().stop();
    }
}

