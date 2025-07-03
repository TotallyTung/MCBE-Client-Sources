//
// Created by Tozic on 9/17/2024.
//

#include "AutoSpellBook.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <Features/Modules/Movement/Speed.hpp>
#include <Features/Modules/Combat/Aura.hpp>

int mHealthDelay = 250;
int mLastHealthSpellUseTime = 0;
int mSpeedDelay = 5000;
int mLastSpeedSpellUseTime = 0;
int mFireTrailDelay = 2000;
int mLastFireTrailSpellUseTime = 0;

void AutoSpellBook::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoSpellBook::onBaseTickEvent, nes::event_priority::VERY_LAST>(this);
}

void AutoSpellBook::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoSpellBook::onBaseTickEvent>(this);
}

int AutoSpellBook::getHealthSpell() {
    auto player = ClientInstance::get()->getLocalPlayer();

    for (int i = 0; i < 8; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Life")) {
            return i;
        }
    }

    return -1;
}

int AutoSpellBook::getSpeedSpell() {
    auto player = ClientInstance::get()->getLocalPlayer();

    for (int i = 0; i < 8; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Swiftness")) {
            return i;
        }
    }

    return -1;
}

int AutoSpellBook::getFireTrailSpell() {
    auto player = ClientInstance::get()->getLocalPlayer();

    for (int i = 0; i < 8; i++) {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;

        if (StringUtils::containsIgnoreCase(item->getCustomName(), "Spell of Fire Trail")) {
            return i;
        }
    }

    return -1;
}

void AutoSpellBook::useSpell(int slot) {
    auto player = ClientInstance::get()->getLocalPlayer();

    auto item = player->getSupplies()->getContainer()->getItem(slot);
    int currentSlot = player->getSupplies()->mSelectedSlot;

    player->getSupplies()->mSelectedSlot = slot;
    player->getGameMode()->baseUseItem(item);
    player->getSupplies()->mSelectedSlot = currentSlot;
}

void AutoSpellBook::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    mHealthSpellSlot = getHealthSpell();
    mSpeedSpellSlot = getSpeedSpell();
    mFireTrailSpellSlot = getFireTrailSpell();

    if(mHealthSpellSlot != -1 && mUseHealthSpell && player->getHealth() < 12) {
        if (mLastHealthSpellUseTime + (uint64_t)mHealthDelay > NOW) return;
        useSpell(mHealthSpellSlot);
        mLastHealthSpellUseTime = NOW;

        if (mShowNotification.mValue) NotifyUtils::notify("Used Health Spell!", 3, Notification::Type::Info);
    }

    if(mSpeedSpellSlot != -1 && mUseSpeedSpell) {
        auto speed = gFeatureManager->mModuleManager->getModule<Speed>();
        if (!speed || !speed->mEnabled) return;
        if (mLastSpeedSpellUseTime + (uint64_t)mSpeedDelay > NOW) return;
        useSpell(mSpeedSpellSlot);
        mLastSpeedSpellUseTime = NOW;

        if (mShowNotification.mValue) NotifyUtils::notify("Used Speed Spell!", 3, Notification::Type::Info);
    }

    if(mFireTrailSpellSlot != -1 && mUseFireTrailSpell) {
        auto aura = gFeatureManager->mModuleManager->getModule<Aura>();
        if (!aura || !aura->sHasTarget) return;
        if (mLastFireTrailSpellUseTime + (uint64_t)mFireTrailDelay > NOW) return;
        useSpell(mFireTrailSpellSlot);
        mLastFireTrailSpellUseTime = NOW;

        if (mShowNotification.mValue) NotifyUtils::notify("Used Fire Trail Spell!", 3, Notification::Type::Info);
    }
};