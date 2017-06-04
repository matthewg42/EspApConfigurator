#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MutilaDebug.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <EspApConfigurator.h>
#include <PersistentSettingString.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingChar.h>

// We set this a little off 0 so the ESP has a chance to connect to
// wifi when first powered on, as the HTTP call is blocking.
unsigned long lastUpload = 10000;

void uploadToThingspeak()
{
    lastUpload = Millis();
    String url = F("http://api.thingspeak.com/update?api_key={k}&field1={1}&field2={2}");
    url.replace("{k}", EspApConfigurator["Thingspeak API Key"]);
    url.replace("{1}", String(random(0,400)));
    url.replace("{2}", String(random(0,400)));
    DB("url: ");
    DBLN(url);

    HTTPClient http;
    http.begin(url);
    // TODO https: http.begin(url, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38");
    int httpCode = http.GET();
    if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DB(F("[HTTP] GET OK, response: "));
            DBLN(payload);
        }
    } else {
        DBF("[HTTP] GET... ERROR: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));
    EspApConfigurator.begin();

    // Must add settings AFTER EspApConfigurator.begin()
    EspApConfigurator.addSetting("Update interval (s)", new PersistentSettingLong(EspApConfigurator.nextFreeAddress(), 15));
    EspApConfigurator.addSetting("Thingspeak API Key", new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 16, "[write api key]"));

    DBLN(F("E:setup"));
}

void loop() 
{
    // Give timeslice
    EspApConfigurator.update();

    // Don't bother trying to upload if we're in Access Point mode, 
    // but we can still do other stuff, for example gathering data
    // because EspApConfigurator doesn't block.  :)
    if (!EspApConfigurator.inApMode()) {
        if (Millis() > lastUpload + EspApConfigurator["Update interval (s)"].toInt()*1000) {
            uploadToThingspeak();
        }
    }
}

