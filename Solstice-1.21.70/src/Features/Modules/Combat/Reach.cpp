//
// Created by alteik on 04/09/2024.
//

#include "Reach.hpp"
#include <SDK/SigManager.hpp>
#include <Utils/Buffer.hpp>
#include <Utils/MemUtils.hpp>

static uintptr_t func = 0x0;

void Reach::onInit() {
    func = SigManager::Reach;

    uintptr_t addr2 = SigManager::BlockReach;
    addr2 += 4;
    int offset2 = *reinterpret_cast<int*>(addr2);
    mBlockReachPtr = reinterpret_cast<float*>(addr2 + offset2 + 4);
}

void Reach::onEnable() {
    MemUtils::ReadBytes((void*)func, mOriginalData, sizeof(mOriginalData));

    mDetour = AllocateBuffer((void*)func);
    MemUtils::writeBytes((uintptr_t)mDetour, mDetourBytes, sizeof(mDetourBytes));

    auto toOriginalAddrRip1 = MemUtils::GetRelativeAddress((uintptr_t)mDetour + sizeof(mDetourBytes) + 1, func + 11);

    MemUtils::writeBytes((uintptr_t)mDetour + sizeof(mDetourBytes), "\xE9", 1);
    MemUtils::writeBytes((uintptr_t)mDetour + sizeof(mDetourBytes) + 1, &toOriginalAddrRip1, sizeof(int32_t));

    auto newRelRip1 = MemUtils::GetRelativeAddress(func + 1, (uintptr_t)mDetour + 4);

    MemUtils::writeBytes(func, "\xE9", 1);
    MemUtils::writeBytes(func + 1, &newRelRip1, sizeof(int32_t));
    MemUtils::NopBytes(func + 5, 6);

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Reach::onBaseTickEvent>(this);
}

void Reach::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Reach::onBaseTickEvent>(this);
    
    MemUtils::writeBytes(func, mOriginalData, sizeof(mOriginalData));
    FreeBuffer(mDetour);

    MemUtils::Write((uintptr_t) mBlockReachPtr, 5.7f);
}

void Reach::onBaseTickEvent(class BaseTickEvent &event) {
    MemUtils::Write((uintptr_t) mDetour, mCombatReach.mValue);
    MemUtils::Write((uintptr_t) mBlockReachPtr, mBlockReach.mValue);
}