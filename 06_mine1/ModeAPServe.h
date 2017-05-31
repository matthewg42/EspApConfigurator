#pragma once

#include "NamedMode.h"

//! \brief Mode which does nothing (except debug out)
class ModeAPServe_ : public NamedMode {
public:
    ModeAPServe_();
    void modeStart();
    void modeUpdate();

};

extern ModeAPServe_ ModeAPServe;

