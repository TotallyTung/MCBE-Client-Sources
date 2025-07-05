//
// Created by vastrakai on 4/14/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class FollowBot : public ModuleBase<FollowBot> {
public:
    FollowBot() : ModuleBase("FollowBot", "Follows a player", "Movement", 0, false) {
    }


    void OnTick() override;
    void OnDisable();

    void OnRender();
};