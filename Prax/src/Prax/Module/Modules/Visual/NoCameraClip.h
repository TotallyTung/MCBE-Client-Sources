#pragma once
//
// Created by vastrakai on 9/5/2023.
//


#include <Prax/Module/Module.h>

class NoCameraClip : public ModuleBase<NoCameraClip> {
public:
    // TODO: Change the byte patch to not interfere with Third Person Zoom (When it's added)
    NoCameraClip() : ModuleBase("NoCameraClip", "Disables camera clipping", "Visual", 0, false) {};

    static uintptr_t CameraClipFunc;

    void OnInitialize() override;
    void OnEnable() override;
    void OnDisable() override;
};