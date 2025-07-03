//
// Created by alteik on 20/10/2024.
//

#pragma once

class NoDebuff : public ModuleBase<NoDebuff> {
public:
    NoDebuff() : ModuleBase<NoDebuff>("NoDebuff", "Prevents the rendering of negative effects", ModuleCategory::Visual, 0, false) {
        mNames = {
            {Lowercase, "nodebuff"},
            {LowercaseSpaced, "no debuff"},
            {Normal, "NoDebuff"},
            {NormalSpaced, "No Debuff"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
};

