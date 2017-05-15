#pragma once

#include <Mode.h>

/*! \class A Mode With Modes
 * What for allowing a major mode to have it's own child modes
 */

class ParentMode : public Mode {
public:
    ParentMode();
    void switchMode(Mode* newMode);
    void modeUpdate();

protected:
    Mode* mode;

};

