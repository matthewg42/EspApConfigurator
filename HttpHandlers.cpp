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

void handleNotFound() {
    DB(F("handleNotFound: "));
    DB(HttpServer.method()==HTTP_GET ? F("GET") : F("POST"));
    DB(' ');
    DBLN(HttpServer.uri());

    String message = "File Not Found\n\n";
    message += "URI: ";
    message += HttpServer.uri();
    message += "\nMethod: ";
    message += ( HttpServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += HttpServer.args();
    message += "\n";

    for ( uint8_t i = 0; i < HttpServer.args(); i++ ) {
        message += " " + HttpServer.argName ( i ) + ": " + HttpServer.arg ( i ) + "\n";
    }
    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
    HttpServer.send (404, "text/plain", message);
    HttpServer.client().stop();
}

void handleSinglePage()
{
    DBLN(F("handleSinglePage"));

    String page = FPSTR(HTTP_HEAD);
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page.replace("{v}", "ESPApConfigurator"); // Page title
    page.replace("{r}", HttpServer.uri());    // Re-load URI
    page += F(("<h1>Configuration</h1>"));

    page += htmlNetworkList();

    page += FPSTR(HTTP_FORM_START);
    page.replace("{a}", "save");

    page += F("<h3>Wifi Credentials</h3>");
    page += FPSTR(HTTP_WIFI_INPUTS);

    // Custom settings
    if (EspApConfigurator.count() > 0) {
        page += F("<h3>Settings</h3>");
        page += htmlSettingsForm();
    }
    
    // End the form with the save button
    page += FPSTR(HTTP_FORM_END);
    page.replace("{s}", "Save & use Wifi");
    if (EspApConfigurator.inApMode()) {
        page.replace("{d}", "Use Wifi (no save)");
    } else {
        page.replace("{d}", "Discard changes");
    }

    page += FPSTR(HTTP_END);

    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
}

void handleSingleSave() {
    DBLN(F("handleSingleSave"));
    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Saving Settings");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);

    String ssid;
    String pass;
    bool ok = true;
    String reason = "";

    if (HttpServer.method() != HTTP_POST) {
        ok = false;
        reason += F("not a POST<br/>");
    } else {
        // we have a POST, good, now populate the parameters
        // iterate over HttpServer.args
        ssid = HttpServer.arg("s");
        pass = HttpServer.arg("p");
        if (ssid == "") {
            ok = false;
            reason = F("no ssid specified<br/>");
        } 

        // Handle custom settings...
        for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
            String id = String('p') + String((char)('a'+i));
            String value = HttpServer.arg(id.c_str());
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
        page += saved;
    }

    page += FPSTR(HTTP_END);

    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
    if (ok) {
        HttpServer.send(200, "text/html", page);
        HttpServer.client().stop();
        ModeWifiClient.setWifiLogin(ssid.c_str(), pass=="" ? NULL : pass.c_str());
        ModeAP.finish();
    } else {
        HttpServer.send(400, "text/html", page);
        HttpServer.client().stop();
    }
}

void handleSingleCancel() {
    DBLN(F("handleSingleCancel"));
    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Cancel Changes");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page += FPSTR(HTTP_NOSAVE);
    page += FPSTR(HTTP_END);

    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
    ModeAP.finish();
}

void handleRescan() {
    DBLN(F("handleRescan"));
    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
    HttpServer.send(200, "text/plain", "ok");
    HttpServer.client().stop();
    ModeAP.startScan();
}

String htmlNetworkList()
{
    String s = F("<h3>Networks</h3>");
    int8_t netCount = WiFi.scanComplete();
    if (netCount>0) {
        for (int8_t i=0; i<netCount; i++) {
            String item = FPSTR(HTTP_WIFI_NET);
            item.replace("{v}", WiFi.SSID(i));
            item.replace("{r}", String(rssiToQuality(WiFi.RSSI(i))));
            if (WiFi.encryptionType(i) != ENC_TYPE_NONE) {
                item.replace("{i}", "l");
            } else {
                item.replace("{i}", "");
            }
            s += item;
        }
    } else {
        s += F("<p>[no networks found]</p>");
    }
    s += F("<button id='r' onclick='r();'>Re-scan WiFi</button>");
    return s;
}

String htmlSettingsForm()
{
    String s = "";
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
        String inp = F("\n<label {lt}for='{i}'>{n}</label>\n<input id='{i}' {it}value='{v}'>{a}\n");
        String id('s'); id += i;
        String labelTags = String();
        String inputTags = String();
        String after = String();
        uint8_t dp;

        switch (EspApConfigurator[i].setting->typecode()[0]) {
        case 'b':
            labelTags = F("class='cb' ");
            inputTags += F("type='checkbox' ");
            inp.replace("{v}", id);
            after = F("<br />");
            break;
        case 'i':
            inputTags += F("type='number' step=1 ");
            break;
        case 'f':
            inputTags += F("type='number' step=0.");
            dp = EspApConfigurator[i].setting->typecode().substring(1).toInt();
            while (dp-- > 1) { inputTags += '0'; }
            inputTags += F("1 ");
            break;
        case 's':
            inputTags += F("type='text' length=");
            inputTags += EspApConfigurator[i].setting->typecode().substring(1);
            inputTags += ' ';
            break;
        default:
            DB(F("WARNING: don't understand setting type: "));
            DBLN(EspApConfigurator[i].setting->typecode());
        }

        inp.replace("{i}", id);
        inp.replace("{n}", EspApConfigurator[i].id);
        inp.replace("{v}", EspApConfigurator[i].setting->get());
        inp.replace("{lt}", labelTags);
        inp.replace("{it}", inputTags);
        inp.replace("{a}", after);

        s += inp;
    }
    return s;
}

