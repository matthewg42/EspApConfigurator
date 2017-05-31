#pragma once

#include <NamedMode.h>

//! \brief Mode which does nothing (except debug out)
class ModeWifiOff_ : public NamedMode {
public:
    ModeWifiOff_();
    void modeStart();
    void modeEnd();
    void modeUpdate();

};

extern ModeWifiOff_ ModeWifiOff;

