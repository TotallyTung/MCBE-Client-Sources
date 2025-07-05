//
// Created by vastrakai on 1/13/2024.
//


#pragma once

#include <Prax/Hook/Hook.h>
#include <Prax/SDK/Game/Misc/Structs.h>


class RakPeerHooks : public Hook
{
public:
    RakPeerHooks() : Hook("RakPeerHooks", true) {};

    /* Static variables */
    static std::unique_ptr<Detour> RunUpdateCycleDetour;
    static int CurrentTick;
    static bool Ticking;
    static int TicksToSkip;
    static int MsToSkip;
    static Vector3 LastPos;
    static bool Visualize;
    static bool RenderCurrentAndLastPos;

    static std::unique_ptr<Detour> GetAveragePingDetour;
    static float LastPing;
    static bool Updated;
    static bool DisableOnTick;

    /* Static functions */
    static void NetSkipDetourFunc(void* _this, void* a2);
    static __int64 GetAveragePingDetourFunc(void* _this, void* a2);
    static void InitAveragePingDetour(uintptr_t addr);

    /* Hook overrides */

    void Init() override;
    void UnHook() override;

};
