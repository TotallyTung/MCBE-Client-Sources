//
// Created by vastrakai on 12/21/2023.
//

#include "RenderItemInHandDescriptionHook.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/SigManager.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/System/Memory.h>
#include <Prax/Module/Modules/Visual/Glint.h>
#include <Prax/Module/Modules/Visual/HUD.h>

class Glint;
std::unique_ptr<Detour> RenderItemInHandDescriptionDetour = nullptr;

void* renderItemInHandDescriptionCtor(RenderItemInHandDescription* _this, void* renderObject, void* itemFlags, void* material, void* glintTexture, void* worldMatrix, bool isDrawingUI, void* globalConstantBuffers, unsigned short viewId, void* renderMetadata)
{
    auto oFunc = RenderItemInHandDescriptionDetour->GetFastcall<void*, RenderItemInHandDescription*, void*, void*, void*, void*, void*, bool, void*, __int16, void*>();
    void* result = oFunc(_this, renderObject, itemFlags, material, glintTexture, worldMatrix, isDrawingUI, globalConstantBuffers, viewId, renderMetadata);

    static auto glint = ModuleManager::GetModule<Glint>();
    if (!glint->Enabled) return result;

    ImColor color = HUD::GetColor(0);
    // Account for Saturation separately
    color.Value.x *= glint->Saturation.Value;
    color.Value.y *= glint->Saturation.Value;
    color.Value.z *= glint->Saturation.Value;

    if (glint->Mode.SelectedIndex == 1) _this->GlintColor = _this->GlintColor + Vector3(color.Value.x, color.Value.y, color.Value.z);
    else _this->GlintColor = Vector3(color.Value.x, color.Value.y, color.Value.z);

    //_this->ChangeColor = Vector3(color.Value.x, color.Value.y, color.Value.z);
    //_this->OverlayColor = Vector3(color.Value.x, color.Value.y, color.Value.z);
    //_this->MultiplicativeTintColor = Vector3(color.Value.x, color.Value.y, color.Value.z);
    return result;
}

void RenderItemInHandDescriptionHook::Init()
{
    uintptr_t func = SigManager::GetSignatureAddress(SignatureEnum::RenderItemInHandDescription_ctor);
    RenderItemInHandDescriptionDetour = std::make_unique<Detour>("RenderItemInHandDescription::ctor", func, renderItemInHandDescriptionCtor);
    RenderItemInHandDescriptionDetour->Enable();
    this->Detours.emplace_back(RenderItemInHandDescriptionDetour.get());
}

void RenderItemInHandDescriptionHook::UnHook() {}
