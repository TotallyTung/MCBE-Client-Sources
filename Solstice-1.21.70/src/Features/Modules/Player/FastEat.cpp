//
// Created by alteik on 11/11/2024.
//

#include "FastEat.hpp"
#include <SDK/SigManager.hpp>
#include <Utils/Buffer.hpp>
#include <Utils/MemUtils.hpp>

static uintptr_t func;

void FastEat::onInit()
{
    //func = SigManager::FastEat;
}

void FastEat::onEnable()
{
    /*MemUtils::ReadBytes((void*)func, mOriginalData, sizeof(mOriginalData));

    mDetour = AllocateBuffer((void*)func);
    MemUtils::writeBytes((uintptr_t)mDetour, mDetourBytes, sizeof(mDetourBytes));

    auto toOriginalAddrRip1 = MemUtils::GetRelativeAddress((uintptr_t)mDetour + sizeof(mDetourBytes) + 1, func + 8);

    MemUtils::writeBytes((uintptr_t)mDetour + sizeof(mDetourBytes), "\xE9", 1);
    MemUtils::writeBytes((uintptr_t)mDetour + sizeof(mDetourBytes) + 1, &toOriginalAddrRip1, sizeof(int32_t));

    auto newRelRip1 = MemUtils::GetRelativeAddress(func + 1, (uintptr_t)mDetour + 4);

    MemUtils::writeBytes(func, "\xE9", 1);
    MemUtils::writeBytes(func + 1, &newRelRip1, sizeof(int32_t));
    MemUtils::NopBytes(func + 5, 3);

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &FastEat::onBaseTickEvent>(this);*/
}

void FastEat::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &FastEat::onBaseTickEvent>(this);

    /*MemUtils::writeBytes(func, mOriginalData, sizeof(mOriginalData));
    FreeBuffer(mDetour);*/
}

void FastEat::onBaseTickEvent(BaseTickEvent& event)
{
    /*int32_t speed = mSpeed.as<int>();
    byte eatSpeed = (speed >= 1 && speed <= 10) ? static_cast<byte>(0x00 + speed) : 0x01;
    MemUtils::writeBytes(reinterpret_cast<uintptr_t>(mDetour), &eatSpeed, sizeof(byte));*/
}
