#pragma once
//
// Created by vastrakai on 10/7/2023.
//

#include <complex.h>
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Hook/Hooks/ContainerScreenControllerHook.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>

class ChestStealer : public ModuleBase<ChestStealer> {
public:
    enum Mode {
        Normal,
        Silent
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode for cheststealer", {"Normal", "Silent" }, 0);
    NumberSetting Delay = NumberSetting("Delay", "The delay to use for cheststealer (in milliseconds)", 0, 1000, 50, 1);
    BoolSetting IgnoreWhileNetskipping = BoolSetting("Ignore While Netskipping", "Ignores cheststealer while netskipping", true);
    BoolSetting IgnoreUselessItems = BoolSetting("Ignore Useless Items", "Ignores items that are not useful", true);

    ChestStealer() : ModuleBase("ChestStealer", "Automatically takes items from chests", "Player", 0, false) {
        AddSetting(&Mode);
        AddSetting(&Delay);
        AddSetting(&IgnoreWhileNetskipping);
        AddSetting(&IgnoreUselessItems);
        Mode.Display = true;
    }

    static bool IsStealing;
    static uint64_t LastItemTaken;
    static bool IsChestOpen;
    static std::vector<NetworkItemStackDescriptor> ItemsToTake;

    void Reset();
    void TakeItem(int slot, NetworkItemStackDescriptor item);
    bool DoDelay() const;
    void OnTick() override;
    static ContainerID CurrentContainerId;
    static uint64_t LastOpen;
    void OnContainerTick(ContainerScreenController *csc) const;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel);
    void OnDisable() override;
};
