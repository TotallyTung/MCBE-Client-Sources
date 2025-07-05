#pragma once

#include "../Hook.h"

class UIRenderCtx : public Hook {
public:
    UIRenderCtx() : Hook("UIRenderCtx") {}


    static void InitVtableHooks(class MinecraftUIRenderContext* ctx);
    void Init() override;
    void UnHook() override;

    static MinecraftUIRenderContext *context;
};