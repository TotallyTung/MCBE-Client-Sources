//
// Created by vastrakai on 7/8/2024.
//

#include "AntiBot.hpp"

#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <Features/Modules/Player/Teams.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Inventory/SimpleContainer.hpp>

void AntiBot::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AntiBot::onBaseTickEvent>(this);
}

void AntiBot::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AntiBot::onBaseTickEvent>(this);
}

void AntiBot::onBaseTickEvent(BaseTickEvent& event)
{
    if (mMode.mValue == Mode::Simple)
    {
        // Set settings to preset values
        mHitboxCheck.mValue = true;
        mPlayerCheck.mValue = true;
        mInvisibleCheck.mValue = true;
        mNameCheck.mValue = true;
    }

    /*auto actors = ActorUtils::getActorList(true);

    for (auto actor : actors)
    {
        if (mPlayerCheck.mValue && actor->isPlayer()) continue;
    }*/
}

constexpr float NORMAL_PLAYER_HEIGHT_MAX = 1.81f;
constexpr float NORMAL_PLAYER_HEIGHT_MIN = 1.35f;
constexpr float NORMAL_PLAYER_WIDTH_MIN = 0.54f;
constexpr float NORMAL_PLAYER_WIDTH_MAX = 0.66f;

std::vector<std::string> AntiBot::getPlayerNames() {
    auto player = ClientInstance::get()->getLocalPlayer();
    std::vector<std::string> playerNames;
    if (!player) return playerNames;

    auto playerList = player->getLevel()->getPlayerList();
    for (const auto& entry : *playerList | std::views::values)
    {
        playerNames.emplace_back(entry.mName);
    }

    return playerNames;
}

bool AntiBot::isBot(Actor* actor)
{
    if (!mEnabled) return false;
    if (mPlayerCheck.mValue && !actor->isPlayer()) return true;

    auto aabbShapeComponent = actor->getAABBShapeComponent();
    if (!aabbShapeComponent) return true;

    float hitboxWidth = aabbShapeComponent->mWidth;
    float hitboxHeight = aabbShapeComponent->mHeight;

    // Return if the hitbox dimensions are incorrect
    if (mHitboxCheck.mValue && (hitboxWidth < NORMAL_PLAYER_WIDTH_MIN || hitboxWidth > NORMAL_PLAYER_WIDTH_MAX ||
                                 hitboxHeight < NORMAL_PLAYER_HEIGHT_MIN || hitboxHeight > NORMAL_PLAYER_HEIGHT_MAX))
        return true;

    if (mInvisibleCheck.mValue && actor->getStatusFlag(ActorFlags::Invisible)) return true;

    if (mNameCheck.mValue) {
        std::string nameTagString = actor->getNameTag();
        if (std::ranges::count(nameTagString, '\n') > 0) {
            return true;
        }
    }

    if (mPlayerListCheck.mValue)
    {
        auto playerList = getPlayerNames();
        std::string nickName = actor->getNameTag();

        if (std::find(playerList.begin(), playerList.end(), nickName) != playerList.end()) {
            return true;
        }
    }

    if (mHasArmorCheck.mValue && !hasArmor(actor)) return true;

    return false;
}

bool AntiBot::hasArmor(Actor* actor)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return false;
    if (!actor->isPlayer()) return false;

    ItemStack* helmetItem = actor->getArmorContainer()->getItem(0);
    ItemStack* chestplateItem = actor->getArmorContainer()->getItem(1);
    ItemStack* legginsItem = actor->getArmorContainer()->getItem(2);
    ItemStack* bootsItem = actor->getArmorContainer()->getItem(3);

    if (mArmorMode.mValue == ArmorMode::Full)
    {
        return helmetItem->mItem && chestplateItem->mItem && legginsItem->mItem && bootsItem->mItem;
    }
    else if (mArmorMode.mValue == ArmorMode::OneElement)
    {
        return helmetItem->mItem || chestplateItem->mItem || legginsItem->mItem || bootsItem->mItem;
    }

    return false;
}