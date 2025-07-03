#pragma once
//
// Created by vastrakai on 11/5/2024.
//


class Desync : public ModuleBase<Desync> {
public:
    NumberSetting mMilliseconds = NumberSetting("Milliseconds", "The amount of milliseconds to desync for", 400, 0, 5000, 1);
    BoolSetting mDebug = BoolSetting("Debug", "Print debug information", false);
    // TODO: Visualize setting

    Desync() : ModuleBase("Desync", "Like Netskip but only for movement.", ModuleCategory::Misc, 0, false) {
        addSettings(&mMilliseconds, &mDebug);

        mNames = {
            {Lowercase, "desync"},
            {LowercaseSpaced, "desync"},
            {Normal, "Desync"},
            {NormalSpaced, "Desync"}
        };
    }

    uint64_t mLastSync = NOW;
    std::vector<std::shared_ptr<class PlayerAuthInputPacket>> mQueuedPackets;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void syncPackets();
    void onPacketOutEvent(class PacketOutEvent& event);
};