#include "NamedMode.h"

NamedMode::NamedMode(const char* name) :
    _name(name)
{
}

const char* NamedMode::name()
{
    return _name.c_str();
}

