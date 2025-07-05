#pragma once
//
// Created by vastrakai on 9/24/2023.
//


#include <Prax/Module/Module.h>


class StreamerMode : public ModuleBase<StreamerMode> {
public:
    StreamerMode() : ModuleBase("StreamerMode", "Hides your name and other information", "Misc", 0, false) {
    }

    static std::string NewName;
    static std::string oldNameTag;
    static std::string nameToReplace;

    void OnInitialize() override;
    void OnEnable() override;
    void OnTick() override;
    void OnDisable() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
};