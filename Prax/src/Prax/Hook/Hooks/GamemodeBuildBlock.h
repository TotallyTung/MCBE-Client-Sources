#pragma once
//
// Created by vastrakai on 9/28/2023.
//


#include <Prax/Hook/Hook.h>

class GamemodeBuildBlock : public Hook {
public:
    GamemodeBuildBlock() : Hook("Gamemode::BuildBlock") {
        InitOnConstruct = false;
    }

    void Init() override;
    void UnHook() override;
};