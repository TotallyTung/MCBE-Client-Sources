#pragma once
//
// Created by vastrakai on 10/3/2023.
//


#include <Prax/Module/Module.h>

class Friends : public ModuleBase<Friends> {
public:
    static bool FriendsEnabled;

    Friends() : ModuleBase("Friends", "Ignores players that are on your friends list", "Player", 0, false) {}

    static bool IsFriend(Actor *actor);
    static bool IsFriend(std::string playerName);
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
};