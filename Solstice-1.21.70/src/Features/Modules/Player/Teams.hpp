//
// Created by alteik on 01/09/2024.
//
#pragma once
#include <Features/Modules/Module.hpp>

class Teams : public ModuleBase<Teams> {
public:
    static inline Teams* instance = nullptr;

    enum class Mode {
        Hive
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the teams module", Mode::Hive, "Hive");

    Teams() : ModuleBase("Teams", "Ignores players that are on your team", ModuleCategory::Player, 0, false) {

        addSettings(&mMode);

        mNames = {
                {Lowercase, "teams"},
                {LowercaseSpaced, "teams"},
                {Normal, "Teams"},
                {NormalSpaced, "Teams"},
        };

        instance = this;
    }

    void onEnable() override;
    void onDisable() override;
    bool isOnTeam(class Actor* actor);
};