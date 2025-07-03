//
// Created by vastrakai on 6/29/2024.
//

#include "ToggleSounds.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftGame.hpp>

void ToggleSounds::onEnable()
{
    gFeatureManager->mDispatcher->listen<ModuleStateChangeEvent, &ToggleSounds::onModuleStateChange>(this);
    gFeatureManager->mDispatcher->listen<ModuleScriptStateChangeEvent, &ToggleSounds::onModuleScriptStateChange>(this);
}

void ToggleSounds::onDisable()
{
    gFeatureManager->mDispatcher->deafen<ModuleStateChangeEvent, &ToggleSounds::onModuleStateChange>(this);
    gFeatureManager->mDispatcher->deafen<ModuleScriptStateChangeEvent, &ToggleSounds::onModuleScriptStateChange>(this);
}

void ToggleSounds::onModuleStateChange(ModuleStateChangeEvent& event)
{
    if (event.isCancelled()) return;

    if (mSound.mValue == Sound::Lever) ClientInstance::get()->getMinecraftGame()->playUi("random.lever_click", mVolume.mValue, event.mEnabled ? 0.6f : 0.5f);
    else if (mSound.mValue == Sound::Smooth) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "smooth_on.wav" : "smooth_off.wav", mVolume.mValue);
    else if (mSound.mValue == Sound::Celestial) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "celestial_on.wav" : "celestial_off.wav", mVolume.mValue);
    else if (mSound.mValue == Sound::Nursultan) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "nursultan_on.wav" : "nursultan_off.wav", mVolume.mValue);
}

void ToggleSounds::onModuleScriptStateChange(ModuleScriptStateChangeEvent& event)
{
    if (event.isCancelled()) return;

    if (mSound.mValue == Sound::Lever) ClientInstance::get()->getMinecraftGame()->playUi("random.lever_click", mVolume.mValue, event.mEnabled ? 0.6f : 0.5f);
    else if (mSound.mValue == Sound::Smooth) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "smooth_on.wav" : "smooth_off.wav", mVolume.mValue);
    else if (mSound.mValue == Sound::Celestial) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "celestial_on.wav" : "celestial_off.wav", mVolume.mValue);
    else if (mSound.mValue == Sound::Nursultan) SoundUtils::playSoundFromEmbeddedResource(event.mEnabled ? "nursultan_on.wav" : "nursultan_off.wav", mVolume.mValue);
}
