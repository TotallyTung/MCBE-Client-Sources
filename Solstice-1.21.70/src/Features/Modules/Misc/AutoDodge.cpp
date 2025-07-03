//
// Created by alteik on 03/09/2024.
//

#include "AutoDodge.hpp"
#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Modules/Misc/AutoQueue.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

void AutoDodge::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoDodge::onPacketInEvent>(this);
}

void AutoDodge::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoDodge::onPacketInEvent>(this);
}

void AutoDodge::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::Text) {
        auto tp = event.getPacket<TextPacket>();

        if(StringUtils::startsWith(tp->mMessage, "§b§l» §r§e"))
        {
            auto mapName = tp->mMessage.substr(8,tp->mMessage.find(" §7") - 8);
            mapName = StringUtils::RemoveColorCodes(mapName);
            mapName = StringUtils::trim(mapName);

            bool requeue = false;

            if (mBalance.mValue && mapName == "Balance")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Balance");
                requeue = true;
            }
            else if (mBaroque.mValue && mapName == "Baroque")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Baroque");
                requeue = true;
            }
            else if (mEuropa.mValue && mapName == "Europa")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Europa");
                requeue = true;
            }
            else if (mGrove.mValue && mapName == "Grove")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Grove");
                requeue = true;
            }
            else if (mIvory.mValue && mapName == "Ivory")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Ivory");
                requeue = true;
            }
            else if (mLush.mValue && mapName == "Lush")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Lush");
                requeue = true;
            }
            else if (mMonastery.mValue && mapName == "Monastery")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Monastery");
                requeue = true;
            }
            else if (mPillars.mValue && mapName == "Pillars")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Pillars");
                requeue = true;
            }
            else if (mPineconePoint.mValue && mapName == "Pinecone Point")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Pinecone Point");
                requeue = true;
            }
            else if (mTurmoil.mValue && mapName == "Turmoil")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Turmoil");
                requeue = true;
            }
            else if (mVillage.mValue && mapName == "Village")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Village");
                requeue = true;
            }
            else if (mVioletKeep.mValue && mapName == "Violet Keep")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Violet Keep");
                requeue = true;
            }
            else if (mVolcano.mValue && mapName == "Volcano")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Volcano");
                requeue = true;
            }
            else if (mWildWood.mValue && mapName == "Wild Wood")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Wild Wood");
                requeue = true;
            }
            else if (mWoodpine.mValue && mapName == "Woodpine")
            {
                ChatUtils::displayClientMessage("§bDodged map §6Woodpine");
                requeue = true;
            }

            if (!requeue) return;

            auto autoQueue = gFeatureManager->mModuleManager->getModule<AutoQueue>();
            if (autoQueue == nullptr)
            {
                ChatUtils::displayClientMessage("§cERROR: AutoQueue module not found.");
                return;
            }

            if (!autoQueue->mEnabled)
            {
                ChatUtils::displayClientMessage("§cAutoQueue is not enabled!");
                return;
            }

            autoQueue->mQueueForGame = true;
            autoQueue->mLastQueueTime = NOW;
        }

    }
}