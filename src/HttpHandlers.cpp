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
    sendStdHeaders();
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
    page.replace("{v}", "EspApConfigurator"); // Page title
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
    page.replace("{a}", "discard");
    page.replace("{s}", "Save & use Wifi");
    if (EspApConfigurator.inApMode()) {
        page.replace("{d}", "Use Wifi (no save)");
    } else {
        page.replace("{d}", "Discard changes");
    }

    page += FPSTR(HTTP_END);

    sendStdHeaders();
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

        // Handle custom settings...
        for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
            String id('s'); id += i;
            String value = HttpServer.arg(id.c_str());

            // handle checkboxes HTML checkboxes - why like this?!
            if (EspApConfigurator[i].setting->typecode() == "b") {
                if (value == id) {
                    value = "1";
                } else {
                    value = "0";
                }
            }

            DB(F("save id="));
            DB(id);
            DB(F(" value="));
            DBLN(value);
            if (!EspApConfigurator[i].setting->set(value)) {
                ok = false;
                reason += F("Setting \"");
                reason += EspApConfigurator[i].id;
                reason += F("\" invalid value \"");
                reason += value;
                reason += "\"<br/>";
                DBLN(" invalid");
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

    sendStdHeaders();
    if (ok) {
        HttpServer.send(200, "text/html", page);
        HttpServer.client().stop();
        if (ssid != "") {
            ModeWifiClient.setWifiLogin(ssid.c_str(), pass=="" ? NULL : pass.c_str());
            ModeAP.finish();
        }
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

    sendStdHeaders();
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
    ModeAP.finish();
}

void handleRescan() {
    DBLN(F("handleRescan"));
    sendStdHeaders();
    HttpServer.send(200, "text/plain", "ok");
    HttpServer.client().stop();
    ModeAP.startScan();
}

void handleLandingPage()
{
    DBLN(F("handleLandingPage"));

    String page = FPSTR(HTTP_HEAD);
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page.replace("{v}", "EspApConfigurator"); // Page title
    page.replace("{r}", HttpServer.uri());    // Re-load URI
    page += F(("<h1>EspApConfigurator</h1>"));

    // Buttons for various sub-pages
    page += F("<a href=\"set\"><button>Project Settings</button></a>");
    page += F("<a href=\"wifi\"><button>WiFi Settings</button></a>");
    page += F("<a href=\"#\"><button>Switch to Wifi</button></a>");

    page += FPSTR(HTTP_END);

    sendStdHeaders();
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
}

void handleWifiPage()
{
    DBLN(F("handleWifiPage"));

    String page = FPSTR(HTTP_HEAD);
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page.replace("{v}", "Wifi Settings");   // Page title
    page.replace("{r}", HttpServer.uri());  // Re-load URI
    page += F(("<h1>Wifi Settings</h1>"));

    page += htmlNetworkList();

    page += FPSTR(HTTP_FORM_START);
    page.replace("{a}", "savewifi");

    page += F("<h3>Wifi Credentials</h3>");
    page += FPSTR(HTTP_WIFI_INPUTS);

    // End the form with the save/cancel buttons
    page += FPSTR(HTTP_FORM_END);
    page.replace("{a}", "cancel");
    page.replace("{s}", "Save");
    page.replace("{d}", "Cancel");

    page += FPSTR(HTTP_END);

    sendStdHeaders();
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
}

void handleSettingsPage()
{
    DBLN(F("handleSettingsPage"));

    String page = FPSTR(HTTP_HEAD);
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += FPSTR(HTTP_HEAD_END);
    page.replace("{v}", "Settings"); // Page title
    page += F(("<h1>Settings</h1>"));

    page += FPSTR(HTTP_FORM_START);
    page.replace("{a}", "saveset");

    // Custom settings
    if (EspApConfigurator.count() > 0) {
        page += F("<h3>Settings</h3>");
        page += htmlSettingsForm();
    }
    
    // End the form with the save/cancel buttons
    page += FPSTR(HTTP_FORM_END);
    page.replace("{a}", "cancel");
    page.replace("{s}", "Save");
    page.replace("{d}", "Cancel");

    page += FPSTR(HTTP_END);

    sendStdHeaders();
    HttpServer.send(200, "text/html", page);
    HttpServer.client().stop();
}

void handleWifiSave()
{
    DBLN(F("handleWifiSave"));
}

void handleSettingsSave()
{
    DBLN(F("handleSettingsSave"));
    handleSingleSave();
}

void handleCancel()
{
    DBLN(F("handleCancel"));

    // just re-direct to langing page
    HttpServer.sendHeader("Location", String("/"), true);
    HttpServer.send( 302, "text/plain", "");

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
        String inp = F("\n<label {lt}for='{i}'>{n}</label>\n<input id='{i}' name='{i}' {it}value='{v}'>{a}\n");
        String id('s'); id += i;
        String labelTags = String();
        String inputTags = String();
        String after = String();
        uint8_t dp;

        switch (EspApConfigurator[i].setting->typecode()[0]) {
        case 'b':
            labelTags = F("class='cb' ");
            inputTags += F("type='checkbox' ");
            DB(F("bool value="));
            DB(EspApConfigurator[i].setting->get());
            if (EspApConfigurator[i].setting->get()!="0") {
                inputTags += F("checked ");
                DBLN(F(" checked"));
            } else {
                DBLN(F(" NOT checked"));
            }
            inp.replace("{v}", id);
            after = F("<br />");
            break;
        case 'c':
            inputTags += F("type='text' length=1 ");
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
        case 't':
            // Time
            inputTags += F("type='text' length=8");
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

void sendStdHeaders()
{
    HttpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    HttpServer.sendHeader("Pragma", "no-cache");
    HttpServer.sendHeader("Expires", "-1");
}
