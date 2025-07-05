//
// Created by Flash on 9/9/2023.
//

#pragma once

#include <Prax/Hook/Hook.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Utils/System/Memory.h>

#include <memory>

class SplashTextRenderer: public Hook {
public:
    SplashTextRenderer() : Hook("SplashTextRenderer") {}

    void Init() override;
    void UnHook() override;

};