//
// Created by vastrakai on 9/5/2023.
//

#include <Prax/SDK/Game/Network/Other/LevelEventPacket.h>
#include "Ambience.h"


void Ambience::OnEnable() {

}

void Ambience::OnDisable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->level->setTime((int)TimeValue.Value);
    player->level->updateWeather(0.f, 0, 0.f, 0);
}

void Ambience::OnInitialize() {

}

void Ambience::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (ChangeTime.Enabled) player->level->setTime((int)TimeValue.Value);
    if (Weather.SelectedIndex > 1) player->level->updateWeather(10.f, Weather.SelectedIndex - 1, 10.f, Weather.SelectedIndex - 1);
    else if (Weather.SelectedIndex == 1) player->level->updateWeather(0.f, 0, 0.f, 0);
}

void Ambience::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (ChangeTime.Enabled && packet->getId() == (int) PacketID::SetTime) *cancel = true;
    if (Weather.SelectedIndex > 0 && packet->getId() == (int) PacketID::LevelEvent)
    {
        auto *lep = (LevelEventPacket *) packet.get();
        if (lep->eventId == LevelEventPacket::EVENT_START_RAIN) *cancel = true;
        if (lep->eventId == LevelEventPacket::EVENT_STOP_RAIN) *cancel = true;
        if (lep->eventId == LevelEventPacket::EVENT_START_THUNDER) *cancel = true;
        if (lep->eventId == LevelEventPacket::EVENT_STOP_THUNDER) *cancel = true;
    }
}

void Ambience::OnRender(){
    /*auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    // this is to prevent the sky from flickering when the time is changed by the server
    if (ChangeTime.Enabled) player->level->setTime((int)TimeValue.Value);*/

}



