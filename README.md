# ESP Access Point Configurator

## Summary

This is a re-implementation and modification of the functionality available in the [WiFiManager library](https://github.com/tzapu/WiFiManager).

Differences:

- Less flash memory usage (faster to upload your code)
- Everything done with timeslices an Mutila Modes, making it possible to run your own code while also being in AP mode (WiFiManager blocks)
- Extensible - add your own configuration options which save to EEPROM

## Status

### Working

- Button press to enter AP mode
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

### TODO

- Make wifi scans much less frequent - behavious should be like this:
    - On enabled AP, do a scan - do not change HeartBeat mode until scan complete
    - Re-scan once a minute, and:
    - Re-scan when user clicks re-scan button in web ui
    - When scanning runs, set HeartBeat to indicate this somehow
- Hostname should not be a default parameter, as ESP does not store it in a non-valatile way
- Enhance settings:
    - allow special setting for hostname.  If it exists, then use it when switching to ModeWifiClient
    - "fingerprinting" EEPROM[0] with forced reset if fingerprint if mismatch
    - each setting type can be queried, and the interface should select an html input as appropriate
- Web UI:
    - ssid/pass pre-populated if the ESP has connected ok in the past
    - pass should have a "show" option
    - setting type-specific inputs
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

