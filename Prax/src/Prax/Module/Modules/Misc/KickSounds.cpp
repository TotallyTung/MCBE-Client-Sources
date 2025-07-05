//
// Created by vastrakai on 1/27/2024.
//

#include "KickSounds.h"

void KickSounds::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::Disconnect)
    {
        Misc::PlaySoundFromUrl("https://github.com/Prax-Client/Releases/raw/main/fardreverb.wav", Volume.Value);
    }
}
