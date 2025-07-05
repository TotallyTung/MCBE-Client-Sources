//
// Created by vastrakai on 2/10/2024.
//


#pragma once
#include <Prax/Hook/Hook.h>

class LevelRendererPlayerHooks : public Hook
{
public:
    LevelRendererPlayerHooks() : Hook("LevelRendererPlayerHooks") {}

    void Init() override;
    void UnHook() override;


};
