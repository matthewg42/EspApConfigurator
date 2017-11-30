# ESP Access Point Configurator

## Summary

This is a re-implementation and modification of the functionality available in the [WiFiManager library](https://github.com/tzapu/WiFiManager).

Differences:

- Less flash memory usage (faster to upload your code)
- Everything done with timeslices and [Mutila library](https://github.com/matthewg42/Mutila/) Modes, making it possible to run your own code while also being in AP mode (WiFiManager blocks)
- Extensible - add your own configuration options which save to EEPROM

## Documentation

Doxygen-generated documentation can be read [here](http://matthewg42.github.io/EspApConfigurator/)

### Notes About the EspApConfigurator Library

The EspApConfigurator library and examples use the [Mutila library](https://github.com/matthewg42/Mutila/) (by the same author) for various functions and classes. To avoid confusion, I'll mention a few here which you may notice in the code:

1. The macros DB, DBLN abd DBF act like Serial.print, Serial.println and Serial.printf respectively. These only produce output if DEBUG is defined, allowing quick enabling / disabling of debugging output on serial when building with the Makefile. To enable debugging output from the IDE, simply add this line at the top of your sketch:

```cpp
#define DEBUG 1
```

2. Mutila objects follow the common Arduino paradigm of having a begin() member which should be called during setup().

3. Many Mutila classes for are implemented with a timeslice approach. That is, they require an update() function to be called frequently to maintain their state. The idea behind this is to avoid using interrupts wherever possible in order to prevent conflicts with other libraries.

4. EspApConfigurator is my own re-implementation of venerable [WiFi Manager](https://github.com/tzapu/WiFiManager) library. It doesn't block like WiFi Manager, and has a better (IMO) persistent settings system. At time of writing, EspApConfigurator is not complete - multi-page setup mode and custom themed settings pages are still to be implemented. However, single page configuration mode works just fine for many projects.

More detailed Doxygen-generated API documentation for the library can be found here: 

* https://matthewg42.github.io/Mutila/
* https://matthewg42.github.io/EspApConfigurator/

## Status

As of Nov 2017, still under frequent development.

### Working

- Button press to enter AP mode (support high=on logic)
- AP mode runs 
- DNS re-directs all to device
- WiFi network scanning
- Web pages get served for setting WiFi Credentials
- Set SSID, passphrase and hostname
- Connect to network in client mode
- Hard-ish reset (zaps SSID from ESP memory)
- Experimental settings for Char, UInt8, Long, String, Float
- Re-arrange project as usable library (wrap whole thing in a ParentMode to it can be easily used in other projects)
- Rescan button (just reloads page)
- Configure settings with web interface 
    - validate on save
    - save to EEPROM
    - validate on load and set default otherwise
- Add default validation (for string)
- No networks found indication 
- Fetch settings by name with operator[](String)
- HTTPS in example code
- Reset nukes all user settings
- Optionally run web interface in ModeWifiClient

### TODO

- Add option to not have heartbeat
- Add ability to remember multiple ssid/passwords
- When connected to WiFi "Save & use WiFi" button should just be a save button
  - in MultiplePade mode, should have a "switch to AP mode" button
- Implement: Multi-page after saving settings re-direct to main page
- Implement: Multi-page after saving wifi re-direct to main page
- Multi-page implement switch to wifi button
- New feature: Add option to disable heartbeat
- Fix: sometimes saving and/connecting results in page load fail - fix it!
- Fix: when in ModeWifiClient and scan is completed, Heartbeat mode doesn't reset properly
- Hostname should not be a default parameter, as ESP does not store it in a non-volatile way
- Enhance settings:
    - allow special setting for hostname.  If it exists, then use it when switching to ModeWifiClient
    - "fingerprinting" EEPROM[0] with forced reset if fingerprint if mismatch
- Web UI:
    - ssid/pass pre-populated if the ESP has connected ok in the past
    - pass should have a "show" option
    - Customised theming
- Better handling of failed connections:
    - If connection fails, retry a few times and then
        - Turn off WiFi and wake it up once ever few minutes to retry
        - monitor button for APmode
- Low power mode:
    - option go into light sleep between periodic activity?
- Static IP config
- Asynchronous HTTP/HTTPS requests (check out https://github.com/me-no-dev/ESPAsyncTCP)
- NTP / time stuff

## Interface Modes

### Mode 1: Single page interface

Typically for projects where web server only runs in ModeAP.  No password protection for changing settings, but ModeAP should have password set.

- Single viewable page
    - WiFi networks list
    - Re-scan button
    - Network settings (ssid, passphrase)
    - Custom settings
    - Save settings button
    - Discard settings button
- Routes: 
    - onNotFound(handleNotFound);
    - on("/",       handleSinglePage);
    - on("/save",   handleSingleSave);
    - on("/wifi",   handleSingleCancel);
    - on("/r",      handleRescan);

### Mode 2: Multi page interface

Typically used for projects where web interface runs in both ModeAP and in ModeWifiClient. 

- Landing page
    - Project name as title
    - Link to WiFi settings page
    - Link to general settings page
- WiFi settings page
    - When in ModeWifiClient
        - Message saying to press the button to switch into ModeAP to change network connection settings
    - When in ModeAP
        - WiFi networks list
        - Re-scan button
        - Network settings (ssid, passphrase)
        - Save settings button
        - Discard settings button
- General settings page
    - Custom settings
    - Save settings button
    - Discard settings button
- Routes: TODO

### Mode 3: Custom interface

User for more complex projects, or when user wants a more customized look and feel. No routes are registered, but there is the option to register handlers by calling HttpServer.addRoute(...)

## Pre-requisites

* Install the Arduino IDE 1.6.6 or later
   * All OSes: [download link](https://www.arduino.cc/en/Main/Software)
   * Major Linux distros carry it in their software repositories, or the link above may be used
* In the Arduino IDE, install ESP8266 board support from Tools -> Board -> Boards Manager
* In the Arduino IDE, install ESP8266 tools from Sketch -> Include Library -> Manage Libraries
* Install the [Mutila](https://github.com/matthewg42/Mutila) library

## Building Using the Arduino IDE

This method is preferred on Windows and for casual users who just want to build and use the project.

* Open Arduino IDE 
* Open firmware/firmware.ino
* Set the board you are using (under the Tools -> Board menu)
* Connect board with USB cable 
* Click the Upload button on the tool bar

## Using make on Linux

This alternative build system may also be used. It is an alternative to using the Arduino IDE and may be preferred by some users. 

* Install the esp/Arduino makefile: https://github.com/plerup/makeEspArduino
* Edit the esp.mk file and set the include line to point at where you installed makeEspArduino.mk (or set the ESP_MAKEFILE environment variable)
* Edit firmware/Makefile and uncomment a BOARD for the board you're using (to see a list of possible boards, run "make list_boards" from the firmware directory)
* Connect your ESP board with a USB cable
* Press and hold the flash button on the board and use the command "make flash" from the firmware directory

