//
// Created by vastrakai on 12/21/2023.
//


#pragma once
#include <Prax/Hook/Hook.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Utils/System/Memory.h>

class RenderItemInHandDescription
{
public:
    BUILD_ACCESS(this, Vector3, GlintColor, 0x8c);
    BUILD_ACCESS(this, float, GlintAlpha, 0x9c);
    BUILD_ACCESS(this, Vector3, OverlayColor, 0xac);
    BUILD_ACCESS(this, float, OverlayAlpha, 0xbc);
    BUILD_ACCESS(this, Vector3, ChangeColor, 0xcc);
    BUILD_ACCESS(this, float, ChangeAlpha, 0xdc);
    BUILD_ACCESS(this, Vector3, MultiplicativeTintColor, 0xec);
    BUILD_ACCESS(this, float, MultiplicativeTintColorAlpha, 0xfc);
};

class RenderItemInHandDescriptionHook : public Hook {
public:
    RenderItemInHandDescriptionHook() : Hook("RenderItemInHandDescription::ctor") {
        InitOnConstruct = false;
    }

    void Init() override;
    void UnHook() override;

};
