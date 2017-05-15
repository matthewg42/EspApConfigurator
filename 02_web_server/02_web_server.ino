#include <ESP8266WiFi.h>
#include <MutilaDebug.h>
#include <Heartbeat.h>

const char *ssid = "ESP AP";

Heartbeat heartbeat(D5);
WiFiServer server(80);

void setup() {
    delay(1000);
    Serial.begin(115200);
    heartbeat.begin();
    heartbeat.setMode(Heartbeat::Normal);
    DBLN();  // escape the junk the ESP8266 puts out when booting
    DB(F("Configuring access point..."));
    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    DB(F("AP IP address: "));
    DBLN(myIP);
    DBLN(F("Starting server..."));
    server.begin();
    DBLN(F("E:setup"));
}

void loop() {
    heartbeat.update();
    WiFiClient client = server.available();
    if (client) {
        heartbeat.setMode(Heartbeat::Quick);
        DBLN(F("new client"));
        while(!client.available()) {
            heartbeat.update();
        }
        String req = client.readStringUntil('\r');
        DBLN(req);
        client.flush();
        heartbeat.setMode(Heartbeat::Normal);
        
        // Whatever the request was, send back Hello World
        String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello World</html>";
        client.print(s);
    }
}

