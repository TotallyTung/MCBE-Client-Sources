//
// Created by vastrakai on 3/2/2024.
//


#pragma once

#include <Prax/Hook/Hook.h>
#include <memory>

class OverworldDimensionHooks : public Hook {
public:
    OverworldDimensionHooks() : Hook("OverworldDimensionHooks") {
        InitOnConstruct = true;
    }
    std::unique_ptr<Detour> OverworldDimensionDetour;

    void Init() override;
    void UnHook() override;
};