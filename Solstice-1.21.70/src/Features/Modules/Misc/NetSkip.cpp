//
// Created by vastrakai on 8/3/2024.
//

#include "NetSkip.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/RunUpdateCycleEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/SetActorMotionPacket.hpp>

void NetSkip::onEnable()
{
    gFeatureManager->mDispatcher->listen<RunUpdateCycleEvent, &NetSkip::onRunUpdateCycleEvent, nes::event_priority::FIRST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &NetSkip::onPacketInEvent>(this);
}

void NetSkip::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RunUpdateCycleEvent, &NetSkip::onRunUpdateCycleEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &NetSkip::onPacketInEvent>(this);
}

void NetSkip::onRunUpdateCycleEvent(RunUpdateCycleEvent& event)
{
    // If damage only is enabled and we are outside the delay, reset the damage flag and return
    if (mDamageOnly.mValue && NOW - mLastDamage > mDamageTime.as<int64_t>())
    {
        mCurrentTick = 0;
        return;
    }

    if (mMode.mValue == Mode::Milliseconds)
    {
        Sleep(mRandomizeDelayMs.mValue ? MathUtils::random(mRandomizeMinMs.as<int>(), mRandomizeMaxMs.as<int>()) : mDelayMs.as<int64_t>());
        event.mApplied = true;
        return;
    }

    if (mCurrentTick < mTickDelay)
    {
        mCurrentTick++;
        event.cancel();
        event.mApplied = true;
        return;
    }

    mCurrentTick = 0;
    mTickDelay = mRandomizeTicks.mValue ? MathUtils::random(mTicksMin.as<int>(), mTicksMax.as<int>()) : mTicks.as<int>();
}

void NetSkip::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::SetActorMotion && mDamageOnly.mValue)
    {
        auto sam = event.getPacket<SetActorMotionPacket>();
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        if (sam->mRuntimeID == player->getRuntimeID())
        {
            mLastDamage = NOW;
        }
    }
}
