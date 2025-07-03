//
// Created by vastrakai on 7/1/2024.
//

#include "Timer.hpp"

#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>

void Timer::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Timer::onBaseTickEvent>(this);
    ClientInstance::get()->getMinecraftSim()->setSimTimer(mSpeed.mValue);
}

void Timer::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Timer::onBaseTickEvent>(this);
    ClientInstance::get()->getMinecraftSim()->setSimTimer(20.0f);
}

void Timer::onBaseTickEvent(BaseTickEvent& event)
{
    ClientInstance::get()->getMinecraftSim()->setSimTimer(mSpeed.mValue);
}
