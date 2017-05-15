# Open AP mode test

## Goal

APMode + WifiClientMode + Basic web server to respond to incoming calls

## Caveats

It would have been nice if the server object could have been a member of the WebClientConnected class, but the callbacks used by ESP8266WebServer don't like member functions (as they have a different calling convention from plain olf functions).  Ho hum.  I created the MyAPI obkect then... must remember to test with multiple connections...
