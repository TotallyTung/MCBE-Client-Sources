#pragma once
//
// Created by vastrakai on 7/18/2024.
//

#include <Hook/Hook.hpp>

class AnimationHooks : public Hook {
public:
    AnimationHooks() : Hook()
    {
        mName = "AnimationHooks";
    }

    static std::unique_ptr<Detour> mSwingDetour;
    static std::unique_ptr<Detour> mBobHurtDetour;


    static int getCurrentSwingDuration(class Actor* actor);
    static void* doBobHurt(void* _this, glm::mat4* matrix);
    void init() override;
};