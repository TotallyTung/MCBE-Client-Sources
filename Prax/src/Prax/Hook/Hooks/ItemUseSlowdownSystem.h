#pragma once
//
// Created by vastrakai on 10/13/2023.
//


#include <Prax/Hook/Hook.h>

class ItemUseSlowdownSystem : public Hook {
public:
    ItemUseSlowdownSystem() : Hook("ItemUseSlowdownSystem") {
        InitOnConstruct = true;
    }

    void Init() override;
    void UnHook() override;
};