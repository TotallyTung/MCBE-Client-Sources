//
// Created by vastrakai on 7/4/2024.
//

#include "KickSounds.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>

void KickSounds::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &KickSounds::onPacketInEvent>(this);
}

void KickSounds::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &KickSounds::onPacketInEvent>(this);
}

void KickSounds::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::Disconnect)
    {
        if (mSound.mValue == Sound::Fard)
        {
            SoundUtils::playSoundFromEmbeddedResource("fard.wav", mVolume.mValue);
        } else if (mSound.mValue == Sound::Mario)
        {
            SoundUtils::playSoundFromEmbeddedResource("mariodeath.wav", mVolume.mValue);
        }
        else if (mSound.mValue == Sound::Windows)
        {
            SoundUtils::playSoundFromEmbeddedResource("windows_background.wav", mVolume.mValue);
        }
    }
}
