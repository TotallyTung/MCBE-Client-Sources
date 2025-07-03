#pragma once
#include <Features/Modules/Module.hpp>
//
// Created by alteik on 12/10/2024.
//

class NameProtect : public ModuleBase<NameProtect> {
public:
    NameProtect() : ModuleBase("NameProtect", "Changes your real name (useful for streaming and similar purposes)", ModuleCategory::Visual, 0, false) {
        mNames = {
                {Lowercase, "nameprotect"},
                {LowercaseSpaced, "name protect"},
                {Normal, "NameProtect"},
                {NormalSpaced, "Name Protect"}
        };

        // god fucking dammit alteik
        mNewName = Solstice::Prefs->mStreamerName;
    }
    std::string mNewName = "";

    std::string mOldLocalName = "";
    std::string mOldNameTag = "";

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};