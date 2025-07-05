//
// Created by vastrakai on 2/10/2024.
//

#include "LevelRendererPlayerHooks.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Module/Modules/Visual/SelectionColor.h>
#include <Prax/SDK/SigManager.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/System/Memory.h>

std::unique_ptr<Detour> renderOutlineSelectionDetour = nullptr;
/*

void __fastcall LevelRendererPlayer::_renderOutlineSelection(
LevelRendererPlayer *this,
ScreenContext *screenContext,
Block *block,
BlockSource *region,
const BlockPos *pos)*/

struct ColorStruct
{
    float r;
    float g;
    float b;
    float a;
};

class StupidClass
{
public:
    BUILD_ACCESS(this, ColorStruct, currentShaderColor, 0x0);
};

class ScreenContext
{
public:
    BUILD_ACCESS(this, StupidClass*, stupid, 0x30);

    void setShaderColor(ColorStruct color)
    {
        // Set the shader color.
        //ColorStruct* color (0x30 offset from this)
        this->stupid->currentShaderColor = color;

    }

};

void* renderOutlineSelectionHook(void* _this, ScreenContext* screenContext, void* block, void* region, void* pos)
{
    auto oFunc = renderOutlineSelectionDetour->GetFastcall<void*, void*, ScreenContext*, void*, void*, void*>();

    if (!ModuleManager::initialized) return oFunc(_this, screenContext, block, region, pos);

    static auto module = ModuleManager::GetModule<SelectionColor>();

    if (!module->Enabled)
    {
        return oFunc(_this, screenContext, block, region, pos);
    }

    ColorStruct color;
    if (module->Color.SelectedIndex == static_cast<int>(SelectionColor::ColorMode::Rainbow))
    {
        ImColor imColor = HUD::GetColor(0);
        color = { imColor.Value.x, imColor.Value.y, imColor.Value.z, 1.f };
    }
    else
    {
        color = { 1.f, 1.f, 1.f, 1.f };
    }

    // Log out color values
    screenContext->setShaderColor({ color.r, color.g, color.b, color.a });


    auto result = oFunc(_this, screenContext, block, region, pos);

    if (module->Enabled)
    {
        screenContext->setShaderColor({ 1.f, 1.f, 1.f, 1.f });
    }

    return result;
}


void LevelRendererPlayerHooks::Init()
{
    uint64_t func = SigManager::GetSignatureAddress(SignatureEnum::LevelRendererPlayer_renderOutlineSelection);

    renderOutlineSelectionDetour = std::make_unique<Detour>("LevelRendererPlayer_renderOutlineSelection", func, &renderOutlineSelectionHook);
    renderOutlineSelectionDetour->Enable();
    this->Detours.emplace_back(renderOutlineSelectionDetour.get());
}

void LevelRendererPlayerHooks::UnHook()
{
}
