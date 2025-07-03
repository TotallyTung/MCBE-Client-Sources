//
// Created by vastrakai on 7/2/2024.
//

#include "LevelInfo.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/SendImmediateEvent.hpp>
#include <Features/Events/PingUpdateEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <Features/Events/PacketInEvent.hpp>

void LevelInfo::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &LevelInfo::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &LevelInfo::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &LevelInfo::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<SendImmediateEvent, &LevelInfo::onSendImmediateEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PingUpdateEvent, &LevelInfo::onPingUpdateEvent, nes::event_priority::VERY_LAST>(this);
}

void LevelInfo::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &LevelInfo::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &LevelInfo::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &LevelInfo::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<SendImmediateEvent, &LevelInfo::onSendImmediateEvent>(this);
    gFeatureManager->mDispatcher->deafen<PingUpdateEvent, &LevelInfo::onPingUpdateEvent>(this);
}

int LevelInfo::getArrowsAmount() {
    auto player = ClientInstance::get()->getLocalPlayer();

    int arrowsCount = 0;
    for (int i = 0; i < 36; i++)
    {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        if (item->getItem()->mName == "arrow")
        {
            arrowsCount += item->mCount;
        }
    }

    return arrowsCount;
}

int LevelInfo::getPearlsAmount() {
    auto player = ClientInstance::get()->getLocalPlayer();

    int pearlsCount = 0;
    for (int i = 0; i < 36; i++)
    {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        if (item->getItem()->mName == "ender_pearl")
        {
            pearlsCount += item->mCount;
        }
    }

    return pearlsCount;
}

int LevelInfo::getSpellsAmount(int mSpellIndex) {
    auto player = ClientInstance::get()->getLocalPlayer();

    int spellsAmount = 0;
    for (int i = 0; i < 36; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        switch (mSpellIndex) {
            case 1: {
                if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Life")) {
                    spellsAmount++;
                }
                break;
            }

            case 2: {
                if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Swiftness")) {
                    spellsAmount++;
                }
                break;
            }

            case 3: {
                if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Fire Trail")) {
                    spellsAmount++;
                }
                break;
            }
        }
    }

    return spellsAmount;
}

void LevelInfo::calculateValue(int mSpellIndex) {

    switch (mSpellIndex) {
        case 1: {
            mHearts = 0;
            for(int i = 0; i < mHealthSpells; i++)
            {
                mHearts += 4;
            }
            break;
        }

        case 2: {
            mSeconds = 0;
            for(int i = 0; i < mSpeedSpells; i++)
            {
                mSeconds += 5;
            }
            break;
        }

        case 3: {
            mBlocks = 0;
            for(int i = 0; i < mFireTrailSpells; i++)
            {
                mBlocks += 10;
            }
            break;
        }
    }
}

void LevelInfo::spellsUpdate() {
    mHealthSpells = getSpellsAmount(1);
    calculateValue(1);

    mSpeedSpells = getSpellsAmount(2);
    calculateValue(2);

    mFireTrailSpells = getSpellsAmount(3);
    calculateValue(3);
}

void LevelInfo::onSendImmediateEvent(SendImmediateEvent& event) {
    uint8_t packetId = event.send[0];
    if (packetId == 0)
    {
        uint64_t timestamp = *reinterpret_cast<uint64_t*>(&event.send[1]);
        uint64_t timestamp64 = _byteswap_uint64(timestamp);
        uint64_t now = NOW;
        mEventDelay = now - timestamp64;
    }
}

void LevelInfo::onPingUpdateEvent(PingUpdateEvent& event)
{
    mPing = event.mPing - mEventDelay;
}


void LevelInfo::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    static glm::vec3 posPrev = *player->getPos();
    glm::vec3 pos = *player->getPos();
    // Ignore y in the BPS calculation

    glm::vec2 posxz = { pos.x, pos.z };
    glm::vec2 posPrevxz = { posPrev.x, posPrev.z };


    float bps = glm::distance(posxz, posPrevxz) * (ClientInstance::get()->getMinecraftSim()->getSimTimer() * ClientInstance::get()->getMinecraftSim()->getSimSpeed());
    mBps = bps;
    mBpsHistory[NOW] = bps;

    for (auto it = mBpsHistory.begin(); it != mBpsHistory.end();)
    {
        if (NOW - it->first > 1000) it = mBpsHistory.erase(it);
        else ++it;
    }

    // Average the BPS
    float total = 0.f;
    int count = 0;
    for (auto it = mBpsHistory.begin(); it != mBpsHistory.end(); ++it)
    {
        total += it->second;
        count++;
    }
    mAveragedBps = total / count;
    posPrev = pos;

    if (mShowArrows) mArrows = getArrowsAmount();
    if (mShowEnderPearls) mPearls = getPearlsAmount();
    if (mShowSpells) spellsUpdate();
}

void LevelInfo::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (ClientInstance::get()->getMouseGrabbed() && player) return;

    FontHelper::pushPrefFont(true);

    ImVec2 pos = { 2, ImGui::GetIO().DisplaySize.y };
    float fontSize = 20.f;
    std::string fpsText = "FPS: " + std::to_string((int)ImGui::GetIO().Framerate);
    float fontY = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, -1.f, fpsText.c_str()).y;
    pos.y -= fontY + 2.f;

    if(mShowSpells.mValue && player)
    {
        if(mShowFireTrailSpells.mValue)
        {
            std::string fireTrailSpellsText = "FireTrail Spells: " + std::to_string(mFireTrailSpells) + " (" + std::to_string(mBlocks) + " blocks)";
            ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), fireTrailSpellsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
            pos.y -= fontY + 2.f;
        }

        if(mShowSpeedSpells.mValue)
        {
            std::string speedSpellsText = "Speed Spells: " + std::to_string(mSpeedSpells) + " (" + std::to_string(mSeconds) + " seconds)";
            ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), speedSpellsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
            pos.y -= fontY + 2.f;
        }

        if(mShowHealthSpells.mValue)
        {
            std::string healthSpellsText = "Health Spells: " + std::to_string(mHealthSpells) + " (" + std::to_string(mHearts) + " hearts)";
            ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), healthSpellsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
            pos.y -= fontY + 2.f;
        }
    }

    if(mShowKicksAmount.mValue && player)
    {
        std::string kicksAmountText = "Kicks Counter: " + std::to_string(mKicksAmount);
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), kicksAmountText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if (mShowBPS.mValue && player)
    {
        std::string bpsText = "BPS: " + fmt::format("{:.2f}", mBps) + " (" + fmt::format("{:.2f}", mAveragedBps) + ")";
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), bpsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if (mShowXYZ.mValue && player)
    {
        glm::ivec3 pPos = *player->getPos();
        std::string xyzText = "XYZ: " + std::to_string(pPos.x) + "/" + std::to_string(pPos.y) + "/" + std::to_string(pPos.z);
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), xyzText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if(mShowArrows.mValue && player)
    {
        std::string arrowsText = "Arrows: " + std::to_string(mArrows);
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), arrowsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if(mShowEnderPearls.mValue && player)
    {
        std::string pearlsText = "Pearls: " + std::to_string(mPearls);
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), pearlsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if (mShowPing.mValue && player)
    {
        std::string pingText = "Ping: " + std::to_string(mPing) + "ms";
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), pingText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }

    if (mShowFPS.mValue && ClientInstance::get()->getScreenName() != "start_screen")
    {
        ImRenderUtils::drawShadowText(ImGui::GetBackgroundDrawList(), fpsText, pos, ImColor(1.f, 1.f, 1.f, 1.f), fontSize);
        pos.y -= fontY + 2.f;
    }


    ImGui::PopFont();

}

void LevelInfo::onPacketInEvent(PacketInEvent &event) {
    if(!mShowKicksAmount.mValue) return;

    if (event.mPacket->getId() == PacketID::Disconnect)
    {
        mKicksAmount++;
    }
}
