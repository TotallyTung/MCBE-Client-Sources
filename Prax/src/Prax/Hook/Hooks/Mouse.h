#pragma once

#include "../Hook.h"

class Mouse : public Hook {
public:
    Mouse() : Hook("Mouse") {}

    void Init() override;
    void UnHook() override;
};