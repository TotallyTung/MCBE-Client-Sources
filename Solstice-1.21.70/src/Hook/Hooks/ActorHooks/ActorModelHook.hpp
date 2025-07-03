#pragma once
//
// Created by vastrakai on 7/25/2024.
//

#include <Hook/Hook.hpp>

class ActorModelHook : public Hook
{
public:
    ActorModelHook() : Hook() {
        mName = "ActorAnimationControllerPlayer::applyToPose";
    }

    static std::unique_ptr<Detour> mDetour;
    // symbol: void ActorAnimationControllerPlayer::applyToPose(RenderParams&, std::unordered_map<SkeletalHierarchyIndex,std::vector<BoneOrientation>>&, float)
    static void onActorModel(uintptr_t a1, uintptr_t a2, uintptr_t a3, float a4, int a5);
    void init() override;
};

