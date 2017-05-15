#pragma once

#include "ParentMode.h"

class NormalMode_ : public ParentMode {
public:
    NormalMode_();
    void begin();
    void modeStart();
    void modeStop();
    void modeUpdate();

};

extern NormalMode_ NormalMode;

