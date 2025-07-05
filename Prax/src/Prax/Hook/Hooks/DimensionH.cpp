//
// Created by vastrakai on 8/10/2023.
//

#include "DimensionH.h"
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/SDK/Game/Entity/Dimension.h>

std::unique_ptr<Detour> GetBrightnessDependentFogColorDetour = nullptr;

rgb* getBrightnessDependentFogColor(Dimension* _this, rgb* color, float a3) {
    auto oFunc = GetBrightnessDependentFogColorDetour.get()->GetFastcall<rgb*, Dimension*, rgb*, float>();


    color = new rgb(0.0f, 0.0f, 0.0f);


    return oFunc(_this, color, a3);
}

void DimensionH::Init() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    if (!Minecraft::ClientInstance->ThePlayer()->dimension) return;
    auto vtable = Minecraft::ClientInstance->ThePlayer()->dimension->vTable;


    GetBrightnessDependentFogColorDetour = std::make_unique<Detour>("GetBrightnessDependentFogColor", vtable[13], &getBrightnessDependentFogColor);
    GetBrightnessDependentFogColorDetour->Enable();

    this->Detours.emplace_back(GetBrightnessDependentFogColorDetour.get());
}

void DimensionH::UnHook() {

}
