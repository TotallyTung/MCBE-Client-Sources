//
// Created by alteik on 02/09/2024.
//

#include "NoHurtcam.hpp"
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

DEFINE_NOP_PATCH_FUNC(hurtTimePatch, SigManager::CameraComponent_applyRotation, 4);

void NoHurtcam::onEnable()
{
    hurtTimePatch(true);
}

void NoHurtcam::onDisable()
{
    hurtTimePatch(false);
}
