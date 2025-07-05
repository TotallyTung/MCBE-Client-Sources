#pragma once
//
// Created by vastrakai on 9/5/2023.
//


#include <Prax/Module/Module.h>

class ShowNametag : public ModuleBase<ShowNametag> {
public:
    ShowNametag() : ModuleBase("ShowNametag", "Shows your local name tag in third-person", "Visual", 0, false) {};

    static uintptr_t NameTagRenderFunc;
    static std::vector<uint8_t> NameTagRenderFuncOriginalBytes;

    void OnInitialize() override;
    void OnEnable() override;
    void OnDisable() override;
};