//
// Created by vastrakai on 11/11/2024.
//

#include "CostumeSpammer.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/CommandRequestPacket.hpp>
#include <SDK/Minecraft/Network/Packets/ModalFormResponsePacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

void CostumeSpammer::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &CostumeSpammer::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &CostumeSpammer::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &CostumeSpammer::onPacketOutEvent>(this);
}

void CostumeSpammer::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &CostumeSpammer::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &CostumeSpammer::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &CostumeSpammer::onPacketOutEvent>(this);
}

void CostumeSpammer::submitForm(int buttonId, int formId)
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

void CostumeSpammer::submitBoolForm(bool buttonId, int formId)
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

void CostumeSpammer::closeForm(int formId)
{
    auto packet = MinecraftPackets::createPacket<ModalFormResponsePacket>();
    packet->mFormId = formId;
    packet->mFormCancelReason = ModalFormCancelReason::UserClosed;
    ClientInstance::get()->getPacketSender()->send(packet.get());
}

void CostumeSpammer::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    int partyItem = -1;
    for (int i = 0; i < 9; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;
        if (StringUtils::containsIgnoreCase(ColorUtils::removeColorCodes(item->getCustomName()), "Global Locker [Use]")) {
            partyItem = i;
            break;
        }
    }

    if (partyItem != -1)
    {
        auto item = player->getSupplies()->getContainer()->getItem(partyItem);
        if (!item->mItem) return;

        if (NOW - mLastInteract > mDelay.mValue * 1000 && mMode.mValue == Mode::Form)
        {
            player->getSupplies()->getContainer()->startUsingItem(partyItem);
            player->getSupplies()->getContainer()->releaseUsingItem(partyItem);
            player->getSupplies()->getContainer()->startUsingItem(partyItem);
            player->getSupplies()->getContainer()->releaseUsingItem(partyItem);
            player->getSupplies()->getContainer()->startUsingItem(partyItem);
            player->getSupplies()->getContainer()->releaseUsingItem(partyItem);
            player->getSupplies()->getContainer()->startUsingItem(partyItem);
            player->getSupplies()->getContainer()->releaseUsingItem(partyItem);
            mLastInteract = NOW;
            // (also make sure the last command was executed more than 500ms ago)
        }
    }

    if (NOW - mLastInteract > mDelay.mValue * 1000 && mMode.mValue == Mode::Command && NOW - mLastCommand > 500)
    {
        CommandUtils::executeCommand("/costume");
        mLastCommand = NOW;
        mLastInteract = NOW;
    }

    for (auto& id : mOpenFormIds) {
        auto json = nlohmann::json::parse(mFormJsons[id]);
        std::string title = json.contains("title") ? json["title"] : "Unknown";
        std::string contentStr = json.contains("content") ? json["content"] : "";

        spdlog::info("Form ID {} with title {} was opened [json: {}]", id, title, mFormJsons[id]);

        if (title.contains("Global Locker") && contentStr.contains("Your global locker contains unlocks that are not game specific"))
        {
            submitForm(2, id);
        }
        if (title.contains("Select")) {
            // Get the amount of available buttons to click, excluding the first(search button) and last (reset button)
            int buttonAmount = json["buttons"].size() - 2;
            int randomButton = MathUtils::random(1, buttonAmount);
            submitForm(randomButton, id);
        }
    }
}

void CostumeSpammer::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::ModalFormRequest) {
        auto packet = event.getPacket<ModalFormRequestPacket>();
        nlohmann::json json = nlohmann::json::parse(packet->mJSON);
        std::string jsonStr = json.dump(4);

        if (!json.contains("title")) return;
        // if the form doesn't contain the title "Global Locker" or "Select" then return

        if (StringUtils::containsIgnoreCase(json["title"], "Global Locker") || StringUtils::containsIgnoreCase(json["title"], "Select"))
        {
            mOpenFormIds.push_back(packet->mFormId);
            mFormJsons[packet->mFormId] = packet->mJSON;
            mFormTitles[packet->mFormId] = json.contains("title") ? json["title"] : "Unknown";

            event.cancel();

            spdlog::info("Form ID {} with title {} was opened", packet->mFormId, mFormTitles[packet->mFormId]);
        } else
        {
            spdlog::info("Form ID {} with title {} was opened", packet->mFormId, json["title"]);
        }
    }



    if (event.mPacket->getId() == PacketID::Text) {
        auto packet = event.getPacket<TextPacket>();
        // Cancel if text contains "Woosh! You now appear as" or "Reset your skin to your Minecraft skin"
        if (StringUtils::containsIgnoreCase(packet->mMessage, "Woosh! You now appear as") || StringUtils::containsIgnoreCase(packet->mMessage, "Reset your skin to your Minecraft skin"))
        {
            event.cancel();
        }
    }
}

void CostumeSpammer::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::ModalFormResponse) {
        auto packet = event.getPacket<ModalFormResponsePacket>();
        if (std::ranges::find(mOpenFormIds, packet->mFormId) != mOpenFormIds.end()) {
            std::erase(mOpenFormIds, packet->mFormId);
        }

        spdlog::info("Form ID {} was closed [{}] [{}]", packet->mFormId, packet->mFormCancelReason.value_or(ModalFormCancelReason::UserClosed) == ModalFormCancelReason::UserClosed ? "UserClosed" : "UserBusy",
            packet->mJSONResponse.has_value() ? packet->mJSONResponse.value().toString() : "No response");
    }

    if (event.mPacket->getId() == PacketID::CommandRequest) {
        mLastCommand = NOW;
    }


}
