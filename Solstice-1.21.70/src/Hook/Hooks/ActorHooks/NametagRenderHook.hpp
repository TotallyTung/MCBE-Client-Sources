#pragma once
//
// Created by vastrakai on 11/10/2024.
//
#include <Hook/Hook.hpp>

class NametagRenderHook : public Hook
{
public:
    NametagRenderHook() : Hook()
    {
        mName = "Unknown::renderNametag";
    }
/*_QWORD *__fastcall sub_140A41910(
        __int64 a1,
        _QWORD *a2,
        __int64 a3,
        __int64 a4,
        _BYTE *a5,
        void *a6,
        __int64 a7,
        char a8,
        int a9,
        __int64 a10)*/
    static std::unique_ptr<Detour> mRenderDetour;

    static void* render(void* a1, void* a2, void* a3, void* a4, class Actor* actor, void* a6, glm::vec3* pos, bool unknown, float deltaThing, mce::Color* color);
    void init() override;
};