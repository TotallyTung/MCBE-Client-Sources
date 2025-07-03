//
// Created by vastrakai on 6/25/2024.
//

#include "BaseTickHook.hpp"

#include <SDK/Minecraft/Actor/Actor.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/OffsetProvider.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

std::unique_ptr<Detour> BaseTickHook::mDetour = nullptr;

void BaseTickHook::onBaseTick(Actor* actor)
{
    auto oFunc = mDetour->getOriginal<&onBaseTick>();
    if (actor != ClientInstance::get()->getLocalPlayer()) return oFunc(actor);

    mQueueMutex.lock();
    auto messages = mQueuedMessages;
    if (!messages.empty())
    {
        std::string messageStr = "";
        for (auto& message : messages) messageStr += message + "\n";
        ClientInstance::get()->getGuiData()->displayClientMessage(messageStr);
        mQueuedMessages.clear();
    }
    mQueueMutex.unlock();

    if (auto supplies = actor->getSupplies())
    {
        supplies->mInHandSlot = supplies->mSelectedSlot;
    }

    for (auto& [mTime, mPacket, mBypassHook] : mQueuedPackets)
    {
        spdlog::trace("Sending packet with ID: {} [queued {}ms ago] [{}]", magic_enum::enum_name(mPacket->getId()), NOW - mTime, mBypassHook ? "bypassing hook" : "not bypassing hook");
        if (mBypassHook) ClientInstance::get()->getPacketSender()->sendToServer(mPacket.get());
        else ClientInstance::get()->getPacketSender()->send(mPacket.get());
    }
    mQueuedPackets.clear();

    static bool once = false;
    if (!once)
    {
        once = true;

        auto holder = nes::make_holder<BaseTickInitEvent>(actor);
        gFeatureManager->mDispatcher->trigger(holder);
    }

    auto holder = nes::make_holder<BaseTickEvent>(actor);
    gFeatureManager->mDispatcher->trigger(holder);

    return oFunc(actor);
}

void BaseTickHook::init()
{
    mDetour = std::make_unique<Detour>("Actor::baseTick", reinterpret_cast<void*>(ClientInstance::get()->getLocalPlayer()->vtable[OffsetProvider::Actor_baseTick]), &BaseTickHook::onBaseTick);
    mDetour->enable();
}
