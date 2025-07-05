//
// Created by Flash on 1/3/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Nametags : public ModuleBase<Nametags> {
public:

    NumberSetting Size = NumberSetting("Size", "Size of the nametag", 1.f, 100.f, 30.f, 1.f);
    BoolSetting HideMinecraftNametags = BoolSetting("Hide Minecraft nametags", "Hides the nametag that Minecraft renders", true);
    BoolSetting ShowFriends = BoolSetting("Show friends", "Show friends", true);
    BoolSetting ShowTeamMembers = BoolSetting("Show team members", "Show team members", true);
    BoolSetting ShowClientName = BoolSetting("Show client name", "Shows their client. This requires IRC to be enabled.", true);
    BoolSetting ShowIRCName = BoolSetting("Show IRC name", "Shows their IRC name. This requires IRC to be enabled.", true);
    BoolSetting Scaling = BoolSetting("Scaling", "Scales the nametag based on distance", true);
    NumberSetting MinimumScale = NumberSetting("Minimum scale", "Minimum scaling font size", 0.1f, 100.f, 10.f, 0.1f);

    Nametags() : ModuleBase("Nametags", "Shows a players name above their player", "Visual", 0, false) {
        AddSetting(&Size);
        AddSetting(&HideMinecraftNametags);
        AddSetting(&ShowFriends);
        AddSetting(&ShowTeamMembers);
        AddSetting(&ShowClientName);
        AddSetting(&ShowIRCName);
        AddSetting(&Scaling);
        AddSetting(&MinimumScale);
    }

    static std::unordered_map<Actor*, std::string> CachedXuids;

    void OnRender() override;
    void OnTick() override;
    void OnEject() override;
    void OnEnable() override;
    void OnDisable() override;
};

