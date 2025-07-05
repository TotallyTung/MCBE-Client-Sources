//
// Created by vastrakai on 11/12/2023.
//


#pragma once
#include <Prax/Hook/Hook.h>

class GamemodeAttack : public Hook {
public:

    void Init() override;
    void UnHook() override;
};
