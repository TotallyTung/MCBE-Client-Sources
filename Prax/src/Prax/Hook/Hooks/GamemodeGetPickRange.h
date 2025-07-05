//
// Created by vastrakai on 1/9/2024.
//


#pragma once

#include <Prax/Hook/Hook.h>

class GamemodeGetPickRange : public Hook {
public:
    GamemodeGetPickRange() : Hook("Gamemode::getPickRange") {
        InitOnConstruct = false;
    }

    static std::unique_ptr<Detour> detour;
    static float detourFunc(void* _this, void* inputMode, bool a3);

    void Init() override;
    void UnHook() override;
};