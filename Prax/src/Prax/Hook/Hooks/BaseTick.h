//
// Created by vastrakai on 8/10/2023.
//
#pragma once

#include <Prax/Hook/Hook.h>
#include <memory>


class BaseTick : public Hook {
public:
    BaseTick() : Hook("BaseTick") {
        InitOnConstruct = false;
    }
    std::unique_ptr<Detour> BaseTickDetour;

    void Init() override;
    void UnHook() override;
};


