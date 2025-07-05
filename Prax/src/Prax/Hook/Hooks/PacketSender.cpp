//
// Created by Flash on 7/30/2023.
//

#include "PacketSender.h"

#include <magic_enum.hpp>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/Command/CommandManager.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Module/Modules/Player/Regen.h>
#include <Prax/Module/Modules/Movement/Speed.h>
#include <Prax/Module/Modules/Visual/ShowRotations.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/Module/Modules/Player/Derp.h>
#include <Prax/Module/Modules/Misc/Disabler.h>
#include <Prax/Module/Modules/World/Scaffold.h>
#include <Prax/Module/Modules/Player/PacketMine.h>
#include <Prax/Utils/Math.h>
#include <Prax/Module/Modules/Misc/StreamerMode.h>
#include <Prax/Module/Modules/Player/MidclickPearl.h>
#include <Prax/SDK/Game/Network/Other/CommandRequestPacket.h>

std::unique_ptr<Detour> PacketSenderDetour = nullptr;

// Define oFunc
typedef void(__fastcall* oFunc)(LoopbackPacketSender*, Packet*);
oFunc sendPacket;
int lastSlot = 0;

int PacketSender::CurrentTick = 0;

void PacketSender::HandleAuthInputTick(PlayerAuthInputPacket* paip, bool* cancel) {
    if (!*cancel)
    {
        if (CurrentTick == 0 && paip->ticksAlive > CurrentTick) CurrentTick = paip->ticksAlive;
        CurrentTick++;
    }

    paip->ticksAlive = CurrentTick;
    if (auto player = Minecraft::ClientInstance->ThePlayer()) {
        player->level->getLevelData()->levelTick = CurrentTick;
    }
}

void PacketSender::SendDetour(LoopbackPacketSender* _this, Packet* packet) {
    // Get ofunc

    if (_this == nullptr)
        return;

    if (packet == nullptr)
        return;

    if (packet->getId() == 0)
    {
        Logger::Write("PacketSender", "WARNING: An invalid packet was sent [getId() == 0]", Logger::LogType::Warning);
        return;
    }

    if (!ModuleManager::initialized)
    {
        // Call ofunc and return
        sendPacket(_this, packet);
        return;
    }



    //auto ofunc = PacketSenderDetour->GetFastcall<void, LoopbackPacketSender*, Packet*>();

    bool cancel = false;

    if (packet->getId() == (int)PacketID::MobEquipment){
        auto mep = (MobEquipmentPacket*)packet;
        if (mep->HotbarSlot == lastSlot) return;
        lastSlot = mep->HotbarSlot;
    }

    if (packet->getId() == (int)PacketID::Text) {
        // Cast packet to TextPacket
        auto textPacket = (TextPacket*)packet;

        auto input = textPacket->message.getText();

        CommandManager::HandleInput(input, &cancel);

    }

    // Call OnPacket for each module

    static auto showRotationsModule = ModuleManager::GetModule<ShowRotations>();

    static std::vector<Module*> modules = {};

    // Sort by priority
    if (modules.empty())
    {
        modules = ModuleManager::Modules;

        std::ranges::sort(modules, [](Module* a, Module* b) {
            return (int)a->PacketPriority < (int)b->PacketPriority;
        });
    }

    for (auto& module : modules)
    {
        if (module == showRotationsModule) continue;

        if (module->Enabled) {
            module->OnPacket(_this, packet, &cancel);
        }
    }



    // if the packet id is PacketID::PlayerAuthInput then make sure the rotations are correct
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket *) packet;
        if (!paip) return;

        HandleAuthInputTick(paip, &cancel); // YEP

        float pitch = paip->rotation.x;
        float yaw = paip->rotation.y;

        Math::Wrap(pitch, -90.f, 90.f);
        Math::Wrap(yaw, -180.f, 180.f);
        // clamp the values too
        Math::Clamp(pitch, -90.f, 90.f);
        Math::Clamp(yaw, -180.f, 180.f);

        paip->rotation.x = pitch;
        paip->rotation.y = yaw;
        float headYawRng = Math::RandomFloat(-2, 2);
        paip->headYaw = yaw + headYawRng;

        // Check the block actions and make sure they are valid
        if (!paip->playerBlockActions.mActions.empty())
        {
            std::vector<PlayerBlockActionData> newActions = {};

            for (auto action : paip->playerBlockActions.mActions)
            {
                bool isQueuedTransac = action.facing > 5 || action.facing < 0;
                if (isQueuedTransac)
                {
                    if (action.facing > 5) action.facing -= 6;
                    else if (action.facing < 0) action.facing += 6;
                }

                newActions.push_back(action);
            }

            paip->playerBlockActions.mActions = newActions;
        }


        static auto derpModule = ModuleManager::GetModule<Derp>();
        if (derpModule->Enabled && derpModule->HeadYaw.Enabled)
        {
            static bool flipDir = false;
            static int derpTick = 0;

            if (derpTick >= (int)(derpModule->FlipTicks.Value) && derpModule->Flip.Enabled) {
                flipDir = !flipDir;
                derpTick = 0;
            }

            if (flipDir) {
                paip->headYaw = paip->rotation.y + 91;
            }
            else {
                paip->headYaw = paip->rotation.y - 91;
            }

            derpTick++;
        }
        if (derpModule->Enabled && derpModule->Mode == Derp::Mode::STATIC)
        {
            paip->rotation.x = 0;
            paip->rotation.y = 0;
            paip->headYaw = 0;
        }
    }

    // Call OnPacket for ShowRotations if it's enabled
    if (showRotationsModule->Enabled) {
        showRotationsModule->OnPacket(_this, packet, &cancel);
    }

    // Call ofunc
    if (!cancel)
        sendPacket(_this, packet);
}

void PacketSender::Init() {

    /// 48 89 ? ? ? 57 48 81 EC C0 00 00 00 0F B6 41 08 48 8B FA 88 42 10 48 8D 54 24 20 48 8B 59 10
    // Hook LoopbackPacketSender::send at index 1

    // wait until we have a client instance
    while (Minecraft::ClientInstance == nullptr) {
        Sleep(100);
    }

    auto vtable = Minecraft::ClientInstance->packetSender->vTable;

    uintptr_t* PacketSenderAddress = vtable[1];


    // use plain minhook
    if (MH_CreateHook((LPVOID)PacketSenderAddress, &SendDetour, reinterpret_cast<LPVOID*>(&sendPacket)) == MH_OK)
        MH_EnableHook((LPVOID)PacketSenderAddress);
}

void PacketSender::UnHook() {

}
