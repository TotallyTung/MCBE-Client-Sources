//
// Created by vastrakai on 7/5/2024.
//

#include "PacketUtils.hpp"

#include <Hook/Hooks/NetworkHooks/PacketReceiveHook.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/MobEquipmentPacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

void PacketUtils::spoofSlot(int slot, bool bypassHook)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto mep = MinecraftPackets::createPacket<MobEquipmentPacket>();

    auto itemStack = player->getSupplies()->getContainer()->getItem(slot);

    mep->mRuntimeId = player->getRuntimeID();
    mep->mItem = NetworkItemStackDescriptor(*itemStack);
    mep->mSlot = slot;
    mep->mSelectedSlot = slot;
    mep->mContainerId = MobEquipmentPacket::ContainerID::Inventory;
    mep->mContainerIdByte = static_cast<unsigned char>(MobEquipmentPacket::ContainerID::Inventory);
    mep->mSelectedSlotByte = static_cast<unsigned char>(slot);
    mep->mSlotByte = static_cast<unsigned char>(slot);
    if(bypassHook) ClientInstance::get()->getPacketSender()->sendToServer(mep.get());
    else ClientInstance::get()->getPacketSender()->send(mep.get());
}

std::shared_ptr<MobEquipmentPacket> PacketUtils::createMobEquipmentPacket(int slot)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto mep = MinecraftPackets::createPacket<MobEquipmentPacket>();

    auto itemStack = player->getSupplies()->getContainer()->getItem(slot);

    mep->mRuntimeId = player->getRuntimeID();
    mep->mItem = NetworkItemStackDescriptor(*itemStack);
    mep->mSlot = slot;
    mep->mSelectedSlot = slot;
    mep->mContainerId = MobEquipmentPacket::ContainerID::Inventory;
    mep->mContainerIdByte = static_cast<unsigned char>(MobEquipmentPacket::ContainerID::Inventory);
    mep->mSelectedSlotByte = static_cast<unsigned char>(slot);
    mep->mSlotByte = static_cast<unsigned char>(slot);

    return mep;
}

void PacketUtils::sendToSelf(const std::shared_ptr<Packet>& packet)
{
    PacketReceiveHook::handlePacket(packet);
}

void PacketUtils::queueSend(std::shared_ptr<Packet> packet, bool bypassHook)
{
    BaseTickHook::mQueuedPackets.emplace_back(packet, bypassHook);
}

void PacketUtils::sendChatMessage(const std::string& msg)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    std::shared_ptr<TextPacket> textPacket = MinecraftPackets::createPacket<TextPacket>();
    textPacket->mLocalize = false;
    textPacket->mMessage = msg;
    textPacket->mType = TextPacketType::Chat;
    textPacket->mAuthor = player->getRawName();
    //textPacket->mXuid = player->getXuid();

    ClientInstance::get()->getPacketSender()->sendToServer(textPacket.get());
}