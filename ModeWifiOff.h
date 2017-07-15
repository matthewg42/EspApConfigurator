#pragma once

#include <Mode.h>

//! \brief Mode which does nothing (except debug out)
class ModeWifiOff_ : public Mode {
public:
    ModeWifiOff_();
    void modeStart();
    void modeUpdate();

};

extern ModeWifiOff_ ModeWifiOff;

