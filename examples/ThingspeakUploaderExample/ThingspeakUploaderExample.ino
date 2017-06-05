#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <EspApConfigurator.h>
#include <MutilaDebug.h>
#include <PersistentSettingChar.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingString.h>
#include <WiFiClientSecure.h>
#include <MemoryFree.h>

// Enable vcc measurement 
ADC_MODE(ADC_VCC);

#define UPLOAD_SERVER   "api.thingspeak.com"
#define CERT_SERVER     "thingspeak.com"
#define UPLOAD_PORT     443
#define URL_TEMPLATE    F("/update?api_key={k}&field1={1}&field2={2}")

unsigned long lastUpload = 0;

void uploadToThingspeak()
{
    lastUpload = Millis();

    WiFiClientSecure client;
    
    DB(F("Opening connection to "));
    DB(UPLOAD_SERVER);
    DB(F(" on port "));
    DB(UPLOAD_PORT);
    DBLN(F("... "));
    if (!client.connect(UPLOAD_SERVER, UPLOAD_PORT)) {
        DBLN(F("FAILED"));
        return;
    }
    DBLN("OK");

    DB(F("Host TLS cert verification (hash: "));
    DB(EspApConfigurator["Certificate Hash"].c_str());
    DB(F(")..."));
    if (!client.verify(EspApConfigurator["Certificate Hash"].c_str(), CERT_SERVER)) {
        DBLN(F("FAILED"));
        return;
    }
    DBLN(F("SUCCESS"));

    String url = URL_TEMPLATE;
    url.replace("{k}", EspApConfigurator["Thingspeak API Key"]);
    url.replace("{1}", String(ESP.getFreeHeap()));
    url.replace("{2}", String(ESP.getVcc()));

    DB(F("url: "));
    DBLN(url);
    DB(F("Sending HTTP request... "));
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                    "Host: " + UPLOAD_SERVER + "\r\n" +
                    "User-Agent: ESP8266/ThingspeakUploaderExample\r\n" +
                    "Connection: close\r\n\r\n");

    bool headers = true;
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            headers = false;
        } else if (headers && line.startsWith("Status:")) {
            DBLN(line);
        } else if (!headers) {
            DB(F("Reply: "));
            DBLN(line);
        }
    }

}

void setup() 
{
    Serial.begin(115200);
    delay(50);
    DBLN(F("\n\nS:setup"));


    EspApConfigurator.begin();

    // Must add settings AFTER EspApConfigurator.begin()
    EspApConfigurator.addSetting("Update interval (s)",   new PersistentSettingLong(EspApConfigurator.nextFreeAddress(), 15));
    EspApConfigurator.addSetting("Thingspeak API Key",    new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 16, "[paste api key]"));
    EspApConfigurator.addSetting("Certificate Hash", new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 60, "[paste fingerprint hash]"));

    // Dump settings
    DBLN(F("Settings:"));
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
        DB(EspApConfigurator[i].id);
        DB(F(" = "));
        DBLN(EspApConfigurator[i].setting->get());
    }

    DBLN(F("E:setup"));
}

void loop() 
{
    // Give timeslice
    EspApConfigurator.update();

    // Don't bother trying to upload until we're connected to a network.
    if (EspApConfigurator.isConnected()) {
        if (lastUpload == 0 || Millis() > lastUpload + EspApConfigurator["Update interval (s)"].toInt()*1000) {
            uploadToThingspeak();
        }
    }
}

