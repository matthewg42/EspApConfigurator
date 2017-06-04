#pragma once

#include "NamedMode.h"

/*! \class A Mode With NamedModes
 * What for allowing a major mode to have it's own child modes
 */

class ParentMode : public NamedMode {
public:
    ParentMode(const char* name);
    void switchMode(NamedMode* newMode);
    void modeUpdate();
    const char* name();

protected:
    NamedMode* pMode;
    String _pname;

};

