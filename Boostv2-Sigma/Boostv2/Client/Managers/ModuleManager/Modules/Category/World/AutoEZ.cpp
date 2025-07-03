#include "AutoEZ.h"
#include <unordered_map>

std::unordered_map<std::string, bool> wasDeadBefore;

AutoEZ::AutoEZ() : Module("AutoEZ", "Automatically says EZ when you kill someone", Category::WORLD) {}

void AutoEZ::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer || !localPlayer->level) return;
    
    for (auto& entity : localPlayer->level->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(entity)) continue;
        if (entity == localPlayer) continue;

        std::string name = *entity->getNameTag();

        if (!entity->isAlive()) {
            if (!wasDeadBefore[name]) {
                std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::Text);
                auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

                pkt->mType = TextPacketType::Chat;
                pkt->mMessage = "Yo " + name + " Your EZ buddy";
                pkt->mPlatformId = "";
                pkt->mLocalize = false;
                pkt->mXuid = "";
                pkt->mAuthor = "";

                Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(pkt);

                wasDeadBefore[name] = true;
            }
        }
        else {
            wasDeadBefore[name] = false;
        }
    }
}
