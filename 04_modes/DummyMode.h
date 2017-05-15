#pragma once

#include <Mode.h>

//! Mode which does nothing, and never ends itself
class DummyMode_ : public Mode {
public:
    DummyMode_();
    void modeStart();
    void modeUpdate();

};

extern DummyMode_ DummyMode;
