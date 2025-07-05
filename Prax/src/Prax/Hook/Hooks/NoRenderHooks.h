//
// Created by vastrakai on 2/4/2024.
//


#pragma once
#include <Prax/Hook/Hook.h>

class NoRenderHooks : public Hook {
public:
    NoRenderHooks() : Hook("NoRenderHooks") {
        InitOnConstruct = true;
    }

    void Init() override;
    void UnHook() override;

};