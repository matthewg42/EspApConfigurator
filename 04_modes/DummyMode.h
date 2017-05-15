#pragma once

#include <Mode.h>

//! \brief Mode which does nothing (except debug out)
class DummyMode_ : public Mode {
public:
    DummyMode_();
    void modeStart();
    void modeUpdate();

};

extern DummyMode_ DummyMode;

