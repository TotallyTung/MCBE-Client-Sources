//
// Created by vastrakai on 8/23/2024.
//

#include "PartySpammer.hpp"

#include <Features/Modules/Combat/Aura.hpp>

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Features/Modules/Misc/Friends.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/ModalFormResponsePacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/World/Level.hpp>

void PartySpammer::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &PartySpammer::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &PartySpammer::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &PartySpammer::onPacketOutEvent>(this);
}

void PartySpammer::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &PartySpammer::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &PartySpammer::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &PartySpammer::onPacketOutEvent>(this);
}

void PartySpammer::submitForm(int buttonId, int formId)
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = formId;
    MinecraftJson::Value json;
    json.mType = MinecraftJson::ValueType::Int;
    json.mValue.mInt = buttonId;
    packet->mJSONResponse = json;

    // Get the assoc button text
    auto jsonObj = nlohmann::json::parse(mFormJsons[formId]);
    try
    {
        std::string buttonText = jsonObj["buttons"][buttonId]["text"];
        spdlog::info("Submitting form with button {}", buttonText);
    } catch (std::exception& e)
    {
        spdlog::error("Failed to get button text: {}", e.what());
    }

    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void PartySpammer::submitBoolForm(bool buttonId, int formId)
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = formId;
    MinecraftJson::Value json;
    json.mType = MinecraftJson::ValueType::Boolean;
    json.mValue.mBool = buttonId;
    packet->mJSONResponse = json;

    // Get the assoc button text
    auto jsonObj = nlohmann::json::parse(mFormJsons[formId]);
    try
    {
        spdlog::info("Submitting form with boolean {}", buttonId);
    } catch (std::exception& e)
    {
        spdlog::error("Failed to get button text: {}", e.what());
    }

    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void PartySpammer::closeForm(int formId)
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = formId;
    packet->mFormCancelReason = ModalFormCancelReason::UserClosed;
    ClientInstance::get()->getPacketSender()->send(packet.get());
}


void PartySpammer::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    if (mMode.mValue == Mode::Form) {
        int partyItem = -1;
        for (int i = 0; i < 9; i++) {
            auto item = player->getSupplies()->getContainer()->getItem(i);
            if (!item->mItem) continue;
            if (StringUtils::containsIgnoreCase(ColorUtils::removeColorCodes(item->getCustomName()), "Party [Use]")) {
                partyItem = i;
                break;
            }
        }

        if (partyItem == -1) return;

        auto item = player->getSupplies()->getContainer()->getItem(partyItem);
        if (!item->mItem) return;

        if (!mActive.mValue) return;

        // interact with party item
        if (NOW - mLastInteract > mDelay.mValue)
        {
            player->getSupplies()->getContainer()->startUsingItem(partyItem);
            player->getSupplies()->getContainer()->releaseUsingItem(partyItem);
            mLastInteract = NOW;
        }

        for (auto& id : mOpenFormIds) {
            auto json = nlohmann::json::parse(mFormJsons[id]);
            std::string title = json.contains("title") ? json["title"] : "Unknown";
            std::string contentStr = json.contains("content") ? json["content"] : "";

            spdlog::info("Form ID {} with title {} was opened [json: {}]", id, title, mFormJsons[id]);

            if (title == "Party" && contentStr.contains("You're not currently in a party!")) {
                submitForm(0, id); // Invite
            }
            else if (title == "Party Invites" && contentStr.contains("or invite by username.")) {
                submitForm(1, id); // Invite all online friends
            }
            // couldn't get submit via string to work :(
            else if (title == "Are you sure?" && contentStr.contains("ALL your online friends")) {
                submitBoolForm(true, id); // Accept
                ChatUtils::displayClientMessage("Party invite sent to all online friends");
                mInteractable = true;
            }
            else if (title == "Party" && contentStr.contains("Use the controls below to manage")) {
                submitForm(2, id); // Disband
                ChatUtils::displayClientMessage("Disbanding party");
                mInteractable = true;
            }
        }
    }
    else if (mMode.mValue == Mode::Command) {
        if (mLastCommandRequestPacketSent + 1000 > NOW) return;

        std::unordered_map<mce::UUID, PlayerListEntry>* playerList = player->getLevel()->getPlayerList();
        std::string names = "";

        int inviteCount = 0;
        for (auto& entry : *playerList | std::views::values)
        {
            if (entry.mName.length() <= 17 && entry.mName != player->getRawName() && !gFriendManager->isFriend(entry.mName))
            {
                names = names + " \"" + entry.mName + "\"";
                inviteCount++;
                if (3 <= inviteCount) break;
            }
        }

        if (!mInvited) {
            CommandUtils::executeCommand("/p invite" + names);
            mInvited = true;
        }
        else {
            CommandUtils::executeCommand("/p disband");
            mInvited = false;
        }
    }
    else if (mMode.mValue == Mode::Aura) {
        if (mLastCommandRequestPacketSent + 1000 > NOW) return;

        if (mInvited) {
            CommandUtils::executeCommand("/p disband");
            mInvited = false;
        }
        else if (Aura::sHasTarget && Aura::sTarget && Aura::sTarget->getActorTypeComponent() && Aura::sTarget->isPlayer()) {
            std::string absoluteName = "\"" + Aura::sTarget->getRawName() + "\"";
            CommandUtils::executeCommand("/p invite " + absoluteName);
            mInvited = true;
        }
    }
}

void PartySpammer::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (mMode.mValue == Mode::Form) {
        if (event.mPacket->getId() == PacketID::ModalFormRequest) {
            int partyItem = -1;
            for (int i = 0; i < 9; i++) {
                auto item = player->getSupplies()->getContainer()->getItem(i);
                if (!item->mItem) continue;
                if (StringUtils::containsIgnoreCase(ColorUtils::removeColorCodes(item->getCustomName()), "Party [Use]")) {
                    partyItem = i;
                    break;
                }
            }

            if (partyItem == -1) return;

            auto packet = event.getPacket<ModalFormRequestPacket>();
            nlohmann::json json = nlohmann::json::parse(packet->mJSON);
            std::string jsonStr = json.dump(4);
            mOpenFormIds.push_back(packet->mFormId);
            mFormJsons[packet->mFormId] = packet->mJSON;
            mFormTitles[packet->mFormId] = json.contains("title") ? json["title"] : "Unknown";

            if (mActive.mValue) event.cancel();

            spdlog::info("Form ID {} with title {} was opened", packet->mFormId, mFormTitles[packet->mFormId]);
        }
    }
    else if (mMode.mValue == Mode::Command) {
        if (event.mPacket->getId() == PacketID::Text) {
            auto textPacket = event.getPacket<TextPacket>();
            if (mHideInviteMessage.mValue && StringUtils::containsIgnoreCase(textPacket->mMessage, "Invited") && StringUtils::containsIgnoreCase(textPacket->mMessage, "to your current party. They have 1 minute to accept.")) {
                event.mCancelled = true;
            }
        }
    }
    else if (mMode.mValue == Mode::Aura) {
        if (event.mPacket->getId() == PacketID::Text) {
            auto textPacket = event.getPacket<TextPacket>();
            if (mHideInviteMessage.mValue) {
                // Invite messages
                if (StringUtils::containsIgnoreCase(textPacket->mMessage, "Invited") && StringUtils::containsIgnoreCase(textPacket->mMessage, "to your current party. They have 1 minute to accept.")) {
                    event.mCancelled = true;
                }

                // Disband messages
                if (StringUtils::equalsIgnoreCase(textPacket->mMessage, "Your party has been disbanded. You are no longer in a party.")) {
                    event.mCancelled = true;
                }
            }
        }
    }
}

void PartySpammer::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::ModalFormResponse && mMode.mValue == Mode::Form) {
        auto packet = event.getPacket<ModalFormResponsePacket>();
        if (std::ranges::find(mOpenFormIds, packet->mFormId) != mOpenFormIds.end()) {
            std::erase(mOpenFormIds, packet->mFormId);
        }

        spdlog::info("Form ID {} was closed [{}] [{}]", packet->mFormId, packet->mFormCancelReason.value_or(ModalFormCancelReason::UserClosed) == ModalFormCancelReason::UserClosed ? "UserClosed" : "UserBusy",
            packet->mJSONResponse.has_value() ? packet->mJSONResponse.value().toString() : "No response");
    }
    if (event.mPacket->getId() == PacketID::CommandRequest && (mMode.mValue == Mode::Command || mMode.mValue == Mode::Aura)) {
        mLastCommandRequestPacketSent = NOW;
    }
}
