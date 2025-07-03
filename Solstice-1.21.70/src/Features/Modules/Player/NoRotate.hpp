#pragma once
//
// Created by ssi on 10/27/2024.
//

#include <Features/Modules/Module.hpp>

class NoRotate : public ModuleBase<NoRotate> {
public:
    NoRotate() : ModuleBase("NoRotate", "Prevents servers from modifying your rotation", ModuleCategory::Player, 0, false) {

        mNames = {
            {Lowercase, "norotate"},
            {LowercaseSpaced, "no rotate"},
            {Normal, "NoRotate"},
            {NormalSpaced, "No Rotate"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
};