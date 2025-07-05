//
// Created by vastrakai on 3/2/2024.
//

#include "OverworldDimensionHooks.h"

#include <Prax/Hook/Detour.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Module/Modules/World/Ambience.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Utils/Render/ColorUtil.h>

std::unique_ptr<Detour> getBrightnessDependentFogColorDetour = nullptr;

ImColor* getBrightnessDependentFogColorDetourFunc(ImColor const& a1, void* a2, void* a3, void* a4)
{
    auto oFunc = getBrightnessDependentFogColorDetour->GetFastcall<ImColor*, ImColor const&, void*, void*, void*>();

    if (!ModuleManager::initialized) return oFunc(a1, a2, a3, a4);

    static auto ambience = ModuleManager::GetModule<Ambience>();


    if (ambience->Enabled && ambience->RainbowSky.Enabled)
    {
        static ImColor fogColor = { 0.0f, 0.0f, 0.0f };
        fogColor = HUD::GetColor(0);
        return &fogColor;
    }

    return oFunc(a1, a2, a3, a4);
}

void OverworldDimensionHooks::Init()
{
    uint64_t func = SigManager::GetSignatureAddress(SignatureEnum::OverworldDimension_getBrightnessDependentFogColor);

    getBrightnessDependentFogColorDetour = std::make_unique<Detour>("OverworldDimension::getBrightnessDependentFogColor", func, &getBrightnessDependentFogColorDetourFunc);
    getBrightnessDependentFogColorDetour->Enable();
    this->Detours.emplace_back(getBrightnessDependentFogColorDetour.get());
}

void OverworldDimensionHooks::UnHook()
{
}
