//
// Created by vastrakai on 1/13/2024.
//

#include "RakPeerHooks.h"

#include <Prax/SDK/SigManager.h>
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/Utils/Misc.h>
#include <Psapi.h>
#include <Prax/ExceptionHandler.h>

std::unique_ptr<Detour> RakPeerHooks::RunUpdateCycleDetour = nullptr;

int RakPeerHooks::CurrentTick = 0;
bool RakPeerHooks::Ticking = false;
int RakPeerHooks::TicksToSkip = 0;
int RakPeerHooks::MsToSkip = 0;
Vector3 RakPeerHooks::LastPos = Vector3(0.f, 0.f, 0.f);
bool RakPeerHooks::Visualize = false;
bool RakPeerHooks::RenderCurrentAndLastPos = false;
std::unique_ptr<Detour> RakPeerHooks::GetAveragePingDetour = nullptr;
float RakPeerHooks::LastPing = 0;
bool RakPeerHooks::Updated = false;
bool RakPeerHooks::DisableOnTick = false;

RakPeerInterface* peer = nullptr;


void RakPeerHooks::NetSkipDetourFunc(void* _this, void* a2)
{
    static bool once = false;
    if (!once) {
        peer = (RakPeerInterface*)_this;

        // Get address of GetAveragePing
        once = true;
    }

    RunUpdateCycleDetour->GetFastcall<void, void*, void*>();
    auto oFunc = RunUpdateCycleDetour.get()->GetFastcall<void, void*, void*>();


    if (!Minecraft::ClientInstance->ThePlayer()) {
        oFunc(_this, a2);
        return;
    }

    if (Ticking && MsToSkip == 0) {
        if (CurrentTick >= TicksToSkip) {
            CurrentTick = 0;
            LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();
            oFunc(_this, a2);
            if (DisableOnTick)
            {
                MsToSkip = 0;
                TicksToSkip = 0;
                Ticking = false;
            }
            return;
        }
        else {
            CurrentTick++;
            return;
        }
    }

    if (Ticking && MsToSkip > 0 && TicksToSkip == 0) {
        LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();
        Sleep(MsToSkip); // We can do this because RunUpdateCycle is called on a separate thread
        oFunc(_this, a2);
        if (DisableOnTick)
        {
            MsToSkip = 0;
            TicksToSkip = 0;
            Ticking = false;
        }

    }

    if (!Ticking) oFunc(_this, a2);
}

__int64 RakPeerHooks::GetAveragePingDetourFunc(void* _this, void* a2)
{
    if (!GetAveragePingDetour) {
        Logger::Write("RakNet", "GetAveragePingDetour is null", Logger::Error);
        Updated = false;
        return 0;
    }
    auto oFunc = GetAveragePingDetour->GetFastcall<__int64, void*, void*>();

    auto value = oFunc(_this, a2);



    LastPing = value;
    Updated = true;

    return value;
}

RakPeerHooks* _thisInstance = nullptr;

void RakPeerHooks::InitAveragePingDetour(uintptr_t addr)
{
    static bool once = false;
    if (once)
    {
        Logger::Write("RakNet", "Already initialized AveragePingDetour", Logger::Error);
        return;
    }
    once = true;

    Logger::Write("RakNet", "Initing GetAveragePingDetour", Logger::Debug);
    GetAveragePingDetour = std::make_unique<Detour>("GetAveragePing", addr, &GetAveragePingDetourFunc);
    GetAveragePingDetour->Enable();
    _thisInstance->Detours.emplace_back(GetAveragePingDetour.get());
}



void RakPeerHooks::Init()
{
    _thisInstance = this;
    uintptr_t netSkipFunc = SigManager::GetSignatureAddress(SignatureEnum::RakNet_RakPeer_runUpdateCycle);

    if (!netSkipFunc) {
        Logger::Write("RakNet", "Failed to find NetSkip function (val: " + Misc::GetModuleOfAddress(netSkipFunc) + ")", Logger::Error);
        return;
    }

    RunUpdateCycleDetour = std::make_unique<Detour>("RunUpdateCycle", netSkipFunc, &NetSkipDetourFunc);
    RunUpdateCycleDetour->Enable();
    this->Detours.emplace_back(RunUpdateCycleDetour.get());

    // Create a new thread to wait for peer to be initialized
    static std::thread t([this]() {
        while (!peer) {
            Sleep(100);
        }
        uintptr_t getAveragePingAddr = Memory::GetAddressByIndex(peer->vTable, 44);
        InitAveragePingDetour(getAveragePingAddr);
        Logger::Write("RakNet", "Initialized RakNet hooks.");
        // Exit thread
        t.detach();
    });
}

void RakPeerHooks::UnHook()
{

}
