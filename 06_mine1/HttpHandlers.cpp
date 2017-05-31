#include <MutilaDebug.h>
#include <Arduino.h>
#include "HttpServer.h"
#include "HttpHandlers.h"

void handleRoot()
{
    DBLN(F("handleRoot"));
     pHttpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
     pHttpServer->sendHeader("Pragma", "no-cache");
     pHttpServer->sendHeader("Expires", "-1");
     pHttpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
     pHttpServer->send(200, "text/html", "");
     pHttpServer->sendContent("<html><head></head><body><h1>HELLO AP!</h1></body></html>");
     pHttpServer->client().stop();
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

