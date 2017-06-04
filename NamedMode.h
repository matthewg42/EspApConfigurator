#pragma once

#include <Arduino.h>
#include <Mode.h>

/*! \class A Mode With A Name
 * What for giving a status to the class user
 */

class NamedMode : public Mode {
public:
    NamedMode(const char* name);
    virtual void modeUpdate() = 0;
    virtual const char* name();

protected:
    String _name;

};

