# ESP Access Point Configurator

## Summary

This is a re-implementation and extension of the functionality available in the [WiFiManager library](https://github.com/tzapu/WiFiManager).

Differences:

- Run your own code while the AP is doing it's thing
- Extensible - add your own configuration options

## Status

This project is under development and is not presently suitable for end users.

### TODO

- Add generic non-volatile settings
   - Integers
   - Floats
   - Fixed length strings
- Wrap whole thing in a ParentMode to it can be easily used in other projects
- Web interface
- Find out what EEPROM addresses are available: answer: 0 to 511. However, it is messy - the ESP EEPROM library works differently from the stock Arduino EEPROM library - it is in fact sort of emulating EEPROM, but is actually flash or something. At any rate, it needs different code from Arduino. I will write some persistent setting classes and maybe have those work on either and then migrate them into the Mutila library. 

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

