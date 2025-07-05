#pragma once
//
// Created by vastrakai on 9/4/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>

class Antibot : public ModuleBase<Antibot>
{
public:
    static inline BoolSetting InvisibleCheckSetting = BoolSetting("Invisible Check", "Excludes the actor if it's invisible", false);
    static inline BoolSetting HitboxCheckSetting = BoolSetting("Hitbox Check", "Excludes the actor if it's hitbox doesn't match a player's hitbox", true);
    static inline BoolSetting IsPlayerCheckSetting = BoolSetting("Is Player Check", "Excludes the actor if it's not a player", true);
    static inline BoolSetting NameCheckSetting = BoolSetting("Name Check", "Excludes the actor if it's name has more than one line", false);
    static inline BoolSetting ExtraNameCheckSetting = BoolSetting("Extra Name Check", "Excludes the actor if it's name doesn't have a color code", false);
    static inline BoolSetting PlayerListCheckSetting = BoolSetting("Player List Check", "Excludes the actor if it's name is not in the player list", false);


    Antibot() : ModuleBase("Antibot", "Attempts to detect bots and remove them from the entitylist", "Combat", 0, true) {
        AddSetting(&InvisibleCheckSetting);
        AddSetting(&HitboxCheckSetting);
        AddSetting(&IsPlayerCheckSetting);
        AddSetting(&NameCheckSetting);
        AddSetting(&ExtraNameCheckSetting);
        AddSetting(&PlayerListCheckSetting);
    };

    static std::vector<Actor*> Actors;
    static bool AntibotEnabled;

    void OnEnable() override;
    void OnDisable() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnTick() override;
    void OnRender() override;

    static bool IsBot(Actor* actor);
    [[nodiscard]] static std::vector<Actor*> GetActors(bool includeTeamMembers = false);

};