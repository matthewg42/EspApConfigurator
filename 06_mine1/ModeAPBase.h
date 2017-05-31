#pragma once

#include "ParentMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeAPBase_ : public ParentMode {
public:
    ModeAPBase_();
    void begin();
    void modeStart();
    void modeUpdate();

protected:
    unsigned long startMs;

};

extern ModeAPBase_ ModeAPBase;

