#pragma once
//
// Created by vastrakai on 7/22/2024.
//

#include <Hook/Hook.hpp>
#include <SDK/Minecraft/Actor/EntityContext.hpp>
#include <SDK/Minecraft/Actor/Components/CameraComponent.hpp>


class LookInputHook : public Hook {
public:
    LookInputHook() : Hook() {
        mName = "CameraDirectLookSystemUtil::_handleLookInput";
    }

    static std::unique_ptr<Detour> mDetour;
    // symbol: void CameraDirectLookSystemUtil::_handleLookInput(EntityContext&, CameraComponent const&, CameraDirectLookComponent&, Vec2 const&)
    static void _handleLookInput(EntityContext* entityContext, CameraComponent& cameraComponent, CameraDirectLookComponent& cameraDirectLookComponent, glm::vec2 const& vec2);
    void init() override;
};
