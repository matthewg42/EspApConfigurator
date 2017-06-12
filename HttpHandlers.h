#pragma once

#include "HttpServer.h"

void handleNotFound();
void handleAllInOnePage();
void handleAllInOneSave();
void handleAllInOneCancel();
void handleRescan();

// HTML taken from MIT-licensed WiFiManager project 
// https://github.com/tzapu/WiFiManager
const char HTTP_HEAD[] PROGMEM        = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_SCRIPT[] PROGMEM      = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();} function r(l) {b=document.getElementById('r');b.innerHTML='scanning...';b.disabled=true;x=new XMLHttpRequest();x.open('GET','r',false);x.send(null);window.setTimeout(function(){console.log('redirect');window.location.href='{r}';},3300);}</script>";
const char HTTP_STYLE[] PROGMEM       = "<style>body{line-height:1.5em;max-width:280px;margin:0 auto;font-family:Helvetica;}button{background-color:#811;color:#fff;margin-top:10px;margin-bottom:10px;padding:0.5rem;border-radius:0.5rem;}.l{background:url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size:1em;}.q{float:right;width:64px;text-align:right;}label,input,button{width:100%;box-sizing:border-box;font-size:1.3rem;border-width:thick;}input{margin-top:5px;margin-bottom:30px;padding:0.5rem;border-radius:0.5rem;}label.cb{width:80%;}input[type=\"checkbox\"]{width:auto;float:right;margin-bottom:20px;transform:scale(1.5);}br{clear:right;}</style>";
const char HTTP_HEAD_END[] PROGMEM    = "</head><body><div class='c'><h1>{v}</h1>";
const char HTTP_WIFI_NET[] PROGMEM    = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_WIFI_FRM_S[] PROGMEM  = "<button id='r' onclick='r();'>Re-scan WiFi</button><form method='post' action='save'><h3>Wifi Settings</h3><p><label for='s'>SSID</label><input id='s' name='s' length=32><label for='p'>Passphrase</label><input id='p' name='p' length=64 type='password'></p>";
const char HTTP_FORM_PARAM[] PROGMEM  = "<label for='{i}'>{p}</label><input id='{i}' name='{i}' length={l} value='{v}'>";
const char HTTP_WIFI_FRM_E[] PROGMEM    = "<h2>Switch to WiFi Mode</h2><button type='submit'>save changes</button></form><br /><form method=\"get\" action=\"wifi\"><button>discard changes</button></form>";
const char HTTP_SAVED[] PROGMEM       = "<div>Settings Saved<br />Trying to connect ESP to network.<br />If connection fails, reconnect to AP to try again</div>";
const char HTTP_NOSAVE[] PROGMEM      = "<div>Settings Not Saved<br />Trying to connect ESP to network.<br />If it fails, reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM         = "</div></body></html>";

