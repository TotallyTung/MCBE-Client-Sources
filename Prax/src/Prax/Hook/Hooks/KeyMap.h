#pragma once

#include "../Hook.h"

class KeyMap : public Hook {
public:
    KeyMap() : Hook("KeyMap") {}

    void Init() override;
    void UnHook() override;
};