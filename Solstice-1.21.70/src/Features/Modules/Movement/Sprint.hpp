#pragma once
//
// Created by vastrakai on 7/10/2024.
//

#include <Features/Modules/Module.hpp>

class Sprint : public ModuleBase<Sprint>
{
public:
    Sprint() : ModuleBase("Sprint", "Automatically sprints", ModuleCategory::Movement, 0, false) {
        mNames = {
            {Lowercase, "sprint"},
            {LowercaseSpaced, "sprint"},
            {Normal, "Sprint"},
            {NormalSpaced, "Sprint"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};