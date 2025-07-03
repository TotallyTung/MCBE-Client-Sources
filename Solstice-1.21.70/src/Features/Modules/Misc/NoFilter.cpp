//
// Created by vastrakai on 7/19/2024.
//

#include "NoFilter.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftGame.hpp>
#include <SDK/Minecraft/UIProfanityContext.hpp>

void NoFilter::onEnable()
{
    ClientInstance::get()->getMinecraftGame()->getProfanityContext()->setEnabled(false);
}

void NoFilter::onDisable()
{
    ClientInstance::get()->getMinecraftGame()->getProfanityContext()->setEnabled(true);
}
