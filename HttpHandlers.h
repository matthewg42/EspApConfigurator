#pragma once

#include "HttpServer.h"

void handleNotFound();
void handleRoot();
void handleWifiSave();
void handleWifi();

// HTML taken from MIT-licensed WiFiManager project 
// https://github.com/tzapu/WiFiManager
const char HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div{padding:3px;font-size:1em;} label,input{float:left;width:90%} input{padding:1em;margin-bottom:1em;border-width:medium;} label,input[type=\"submit\"]{clear:left} body{text-align: center;font-family:verdana;} button{border:1em;border-radius:0.3rem;background-color:#811;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method=\"get\" action=""><button>Re-scan Networks</button></form><form method='post' action='save'><h3>Wifi Settings</h3><p><label for='s'>SSID</label><input id='s' name='s' length=32><label for='p'>Passphrase</label><input id='p' name='p' length=64 type='password'><label for='h'>Hostname</label><input id='h' name='h' length=32 value='{h}'></p>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<label for='{i}'>{p}</label><input id='{i}' name='{i}' length={l} value='{v}'>";
const char HTTP_FORM_END[] PROGMEM        = "<h2>Switch to WiFi Mode</h2><button type='submit'>save changes</button></form><br /><form method=\"get\" action=\"wifi\"><button>discard changes</button></form>";
const char HTTP_SAVED[] PROGMEM           = "<div>Settings Saved<br />Trying to connect ESP to network.<br />Device will have hostname {h}<br />If it fails, reconnect to AP to try again</div>";
const char HTTP_NOSAVE[] PROGMEM           = "<div>Settings Not Saved<br />Trying to connect ESP to network.<br />If it fails, reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

