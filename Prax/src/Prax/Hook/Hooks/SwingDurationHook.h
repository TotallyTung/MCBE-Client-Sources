#pragma once
//
// Created by vastrakai on 9/23/2023.
//


#include <Prax/Hook/Hook.h>

class SwingDurationHook : public Hook {
public:
    SwingDurationHook() : Hook("SwingDurationHook") {
        InitOnConstruct = true;
    }

    void Init() override;
    void UnHook() override;

};