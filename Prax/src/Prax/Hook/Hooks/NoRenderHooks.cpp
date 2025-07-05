//
// Created by vastrakai on 2/4/2024.
//

#include "NoRenderHooks.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/NoRender.h>
#include <Prax/SDK/SigManager.h>

std::unique_ptr<Detour> NoRenderDetour;

void* noRenderDetourFunc(void* a1, void* a2)
{
    auto oFunc = NoRenderDetour->GetFastcall<void*, void*, void*>();

    if (!ModuleManager::initialized) return oFunc(a1, a2);
    auto noRenderModule = ModuleManager::GetModule<NoRender>();

    if (noRenderModule->Enabled && noRenderModule->Particles.Enabled) return nullptr;

    return oFunc(a1, a2);
}

void NoRenderHooks::Init()
{
    uint64_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::NoRenderParticles));

    NoRenderDetour = std::make_unique<Detour>("NoRenderParticles", func, &noRenderDetourFunc);
    NoRenderDetour->Enable();
    this->Detours.emplace_back(NoRenderDetour.get());
}

void NoRenderHooks::UnHook()
{

}
