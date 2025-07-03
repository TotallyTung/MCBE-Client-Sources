#pragma once
//
// Created by vastrakai on 7/7/2024.
//
#include <Hook/Hook.hpp>
#include <Hook/HookManager.hpp>
#include <SDK/Minecraft/mce.hpp>


class SetupAndRenderHook : public Hook {
public:
    SetupAndRenderHook() : Hook() {
        mName = "ScreenView::setupAndRender";
    }

    static std::unique_ptr<Detour> mSetupAndRenderDetour;
    static std::unique_ptr<Detour> mDrawImageDetour;


    static void* onSetupAndRender(void* screenView, void* mcuirc);
    static void* onDrawImage(void* context, mce::TexturePtr* texture, glm::vec2* pos, glm::vec2* size, glm::vec2* uv, mce::Color* color, void* unk);
    static void initVt(void* ctx);
    void init() override;
};

