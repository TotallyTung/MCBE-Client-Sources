#pragma once

#include "../../Module.h"

class ClickGui2 : public Module {
public:
    ClickGui2(Category* c) : Module(c, "ClickGui2", "v2 clickgui") {
        this->key = 'L';
    };
private:
    bool setWinAttr;
};