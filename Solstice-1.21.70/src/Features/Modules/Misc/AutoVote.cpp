//
// Created by ssi on 10/25/2024.
//

#include "AutoVote.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/ModalFormResponsePacket.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>

void AutoVote::onEnable() {
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoVote::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoVote::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoVote::onBaseTickEvent>(this);
}

void AutoVote::onDisable() {
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoVote::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoVote::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoVote::onBaseTickEvent>(this);
}

void AutoVote::submitForm(int buttonId) {
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = mLastFormId;

    MinecraftJson::Value json;
    json.mType = MinecraftJson::ValueType::Int;
    json.mValue.mInt = buttonId;
    packet->mJSONResponse = json;

    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void AutoVote::closeForm() {
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = mLastFormId;
    packet->mFormCancelReason = ModalFormCancelReason::UserClosed;

    ClientInstance::get()->getPacketSender()->send(packet.get());
}

std::vector<std::string> getPriorityMaps() {
    std::vector<std::string> maps;
    std::ifstream file(FileUtils::getSolsticeDir() + "Templates\\autoVoteTemplate.txt");

    if (!file.is_open()) return maps;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '/' && line[0] != '\n') {
            maps.push_back(line);
        }
    }

    return maps;
}

void AutoVote::onBaseTickEvent(BaseTickEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    int voteItem = -1;
    for (int i = 0; i < 9; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (item->mItem && StringUtils::containsIgnoreCase(ColorUtils::removeColorCodes(item->getCustomName()), "Vote for Map [Use]")) {
            voteItem = i;
            break;
        }
    }

    if (voteItem == -1) return;

    if (mHasFormOpen && !mVotedThisDimension) {
        auto jsonObj = nlohmann::json::parse(mJson);
        auto priorityMaps = getPriorityMaps();

        std::string type = jsonObj.value("type", "");
        std::string title = jsonObj.value("title", "");

        if (!(type == "form" && StringUtils::containsIgnoreCase(title, "Choose Map"))) return;

        if (jsonObj.contains("buttons")) {
            size_t buttonCount = jsonObj["buttons"].size();
            if (buttonCount > 0) {
                int chosenButton = -1;
                bool mapFound = false;
                std::string chosenMapName;

                if (mPrioritizeVoting.mValue && !priorityMaps.empty()) {
                    for (const auto& mapName : priorityMaps) {
                        for (size_t i = 0; i < buttonCount; ++i) {
                            if (jsonObj["buttons"][i]["text"] == mapName) {
                                chosenButton = i;
                                chosenMapName = mapName;
                                mapFound = true;
                                break;
                            }
                        }
                        if (mapFound) break;
                    }

                    if (!mapFound) {
                        ChatUtils::displayClientMessage("§cNone of your preferred maps are available for voting.");
                        closeForm();
                        mHasFormOpen = false;
                        mVotedThisDimension = true;
                        return;
                    }
                } else {
                    chosenButton = rand() % buttonCount;
                    chosenMapName = jsonObj["buttons"][chosenButton]["text"];
                }

                // ChatUtils::displayClientMessage("§aVoted for: " + chosenMapName);
                submitForm(chosenButton);
                closeForm();
                mHasFormOpen = false;
                mVotedThisDimension = true;
            } else {
                ChatUtils::displayClientMessage("§cNo maps available to vote for.");
            }
        }
    } else if (!mHasFormOpen && !mVotedThisDimension && !mInteractedThisDimension) {
        player->getSupplies()->getContainer()->startUsingItem(voteItem);
        player->getSupplies()->getContainer()->releaseUsingItem(voteItem);
        mInteractedThisDimension = true;
    }
}

void AutoVote::onPacketOutEvent(PacketOutEvent& event) {
    if (event.mPacket->getId() == PacketID::ModalFormResponse) {
        auto packet = event.getPacket<ModalFormResponsePacket>();
        if (packet->mFormId == mLastFormId) {
            mHasFormOpen = false;
        }
    }
}

void AutoVote::onPacketInEvent(PacketInEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::ModalFormRequest) {
        auto packet = event.getPacket<ModalFormRequestPacket>();
        mJson = packet->mJSON;
        mLastFormId = packet->mFormId;
        mHasFormOpen = true;

        auto jsonObj = nlohmann::json::parse(mJson);
        if (jsonObj.contains("type") && StringUtils::containsIgnoreCase(jsonObj["title"], "Choose Map") && !mVotedThisDimension) {
            event.cancel();
        }
    }

    if (event.mPacket->getId() == PacketID::ChangeDimension) {
        mHasFormOpen = false;
        mVotedThisDimension = false;
        mInteractedThisDimension = false;
        mLastFormId = -1;
    }
}