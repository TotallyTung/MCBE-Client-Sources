//
// Created by vastrakai on 9/14/2023.
//
#include "PacketHandler.h"

#include <magic_enum.hpp>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/Modules/Movement/Velocity.h>
#include <Prax/SDK/Game/Network/Other/DisconnectPacket.h>
#include <Prax/SDK/Game/Network/Other/UpdateBlockPacket.h>


std::map<PacketID, std::unique_ptr<Detour>> PacketHandler::PacketHandlerDetours = std::map<PacketID, std::unique_ptr<Detour>>();
void* PacketHandler::NetworkIdentifier = nullptr;

// PacketHandlerDispatcherInstance<MovePlayerPacket,false>::handle(NetworkIdentifier const&,NetEventCallback &,std::__ndk1::shared_ptr<Packet> &)
void* packetHandlerDetour(void* _this, void* networkIdentifier, void* netEventCallback, std::shared_ptr<Packet> packet) {
    // Get ofunc
    auto ofunc = PacketHandler::GetOfunc(packet->getId());

    PacketHandler::NetworkIdentifier = networkIdentifier;

    bool cancel = false;

    // Call OnPacketReceive
    if (packet->getId() == (int)PacketID::ChangeDimension)
    {

        std::vector<Module*> toggledModules = {};
        for (auto& module : ModuleManager::Modules) {
            if (module->Enabled && module->Name != "Velocity") {
                module->OnPacketReceive(packet, &cancel);

                if (module->DisableOnDimensionSwitch)
                {
                    module->Toggle();
                    toggledModules.push_back(module);
                }
            }
        }

        // Find all the modules on the same KeyId as the module that was toggled and add them to the otherModules vector
        std::vector<Module*> otherModules = {};
        for (auto& module : ModuleManager::Modules)
        {
            if (toggledModules.empty()) break;
            if (module->KeyId == toggledModules[0]->KeyId) {
                // Make sure the module wasnt already toggled. If it was, continue
                if (std::ranges::find(toggledModules, module) != toggledModules.end()) continue;
                otherModules.push_back(module);
            }
        }

        // Toggle the other modules
        for (auto& module : otherModules) {
            module->Toggle();
        }

    } else {
        for (auto& module : ModuleManager::Modules) {
            if (module->Enabled && module->Name != "Velocity" && module->Name != "TestFly") {
                module->OnPacketReceive(packet, &cancel);
            }
            else if (module->Name == "TestFly") { // always call regardless of enabled state
                module->OnPacketReceive(packet, &cancel);
            }
        }
    }

    auto velocityModule = ModuleManager::GetModule<Velocity>();
    if (velocityModule && velocityModule->Enabled) {
        velocityModule->OnPacketReceive(packet, &cancel);
    }


#ifdef __RELEASEPAID__
    // I hate you amaar
    if (packet->getId() == (int)PacketID::Disconnect)
    {
        auto dp = std::reinterpret_pointer_cast<DisconnectPacket>(packet);
        // Sanitize the disconnect message
        std::string reason = dp->message;
        // Make sure the reason starts with "Error: " (The rest of the msg is a base64 code)
        // Remove the "Error: " part
        if (String::ContainsIgnoreCase(reason, "Error: "))
        {
            reason = reason.substr(10);

            Misc::SetClipboard(reason);
        }
    }
#endif

    // log each param

    if (!cancel) return ofunc(_this, networkIdentifier, netEventCallback, packet);


    return nullptr;
}

// Add all the packets you want to hook here
std::vector<PacketID> HookedIDs = {
        PacketID::Text,
        PacketID::MovePlayer,
        PacketID::PlayerAuthInput,
        PacketID::PlayerAction,
        PacketID::Disconnect,
        PacketID::ChangeDimension,
        PacketID::SetTime,
        PacketID::LevelEvent,
        PacketID::CommandOutput,
        PacketID::CommandBlockUpdate,
        PacketID::SetEntityMotion,
        PacketID::SetEntityLink,
        PacketID::SetHealth,
        PacketID::UpdateAttributes,
        PacketID::EntityEvent,
        PacketID::RemoveEntity,
        PacketID::NetworkStackLatency,
        PacketID::UpdateBlock,
        PacketID::UpdateAbilitiesPacket,
        PacketID::ModelFormRequest,
        PacketID::InventoryContent,
};


void PacketHandler::Init() {
    bool hookAll = true;

    if (hookAll)
    {
        uint64_t start = Misc::GetCurrentMs();
        // Get the number of hardware threads (cores) available
        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) {
            numThreads = 1; // Fallback to 1 thread if hardware_concurrency() is not supported
        }

        // Vector to hold all the future objects
        std::vector<std::future<void>> futures;

        // Get all packet IDs
        auto packetIDs = magic_enum::enum_values<PacketID>();

        // Determine the size of each chunk
        size_t chunkSize = (packetIDs.size() + numThreads - 1) / numThreads;

        // Function to handle packet creation and detouring for a chunk of packet IDs
        auto handlePacketChunk = [this](auto begin, auto end) {
            for (auto it = begin; it != end; ++it) {
                int i = static_cast<int>(*it);
                auto packet = MinecraftPackets::CreatePacket(i);
                if (!packet) {
                    Logger::Write("PacketHandler", "Failed to create packet 0x" + String::ToHex(i), Logger::LogType::Debug);
                    continue;
                }

                PacketHandlerDetours[*it] = std::make_unique<Detour>(
                    "Packet" + String::ToHex(i) + "HandlerDetour",
                    reinterpret_cast<void*>(packet->getHandleFunc()),
                    reinterpret_cast<void*>(packetHandlerDetour), true);
                PacketHandlerDetours[*it]->Enable(true);
                this->Detours.emplace_back(PacketHandlerDetours[*it].get());
            }
        };

        // Launch async tasks for each chunk
        for (size_t i = 0; i < packetIDs.size(); i += chunkSize) {
            auto begin = packetIDs.begin() + i;
            auto end = (i + chunkSize < packetIDs.size()) ? (packetIDs.begin() + i + chunkSize) : packetIDs.end();
            futures.push_back(std::async(std::launch::async, handlePacketChunk, begin, end));
        }

        // Wait for all async tasks to complete
        for (auto& future : futures) {
            future.get();
            Logger::Write("PacketHandler", "Hooked " + std::to_string(PacketHandlerDetours.size()) + " packets\r", Logger::LogType::Debug, false);
        }

        uint64_t time = Misc::GetCurrentMs() - start;



        Logger::Write("PacketHandler", "Successfully hooked " + std::to_string(PacketHandlerDetours.size()) + " packets in " + std::to_string(time) + "ms", Logger::LogType::Info);
    }
    else
        for (PacketID id: HookedIDs) {
            int i = (int) id;
            auto packet = MinecraftPackets::CreatePacket(i);
            if (!packet) {
                Logger::Write("PacketHandler", "Failed to create packet 0x" + String::ToHex(i), Logger::LogType::Debug);
                continue;
            }

            PacketHandlerDetours[(PacketID) i] = std::make_unique<Detour>(
                    "Packet" + String::ToHex(i) + "HandlerDetour",
                    (void *) packet->getHandleFunc(),
                    (void *) packetHandlerDetour);
            PacketHandlerDetours[(PacketID) i]->Enable();
            this->Detours.emplace_back(PacketHandlerDetours[(PacketID) i].get());
        }
}

void PacketHandler::UnHook() {

}
