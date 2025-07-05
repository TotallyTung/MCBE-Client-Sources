//
// Created by vastrakai on 1/7/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class SafeWalk : public ModuleBase<SafeWalk> {
public:
    SafeWalk() : ModuleBase("SafeWalk", "Prevents you from falling off blocks", "Movement", 0, false) {}

    static uintptr_t SafeWalkAddress;
    static std::vector<char> NewBytes;
    static std::vector<char> OgBytes;

    void OnTick() override;
    void OnDisable() override;
    void OnEnable() override;
    void OnInitialize() override;
    void OnEject() override;

};