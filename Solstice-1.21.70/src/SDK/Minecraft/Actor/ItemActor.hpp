#pragma once
//
// Created by vastrakai on 8/7/2024.
//

#include "Actor.hpp"
#include <SDK/Minecraft/Inventory/ItemStack.hpp>

class ItemActor : public Actor {
public:
    ItemStack mItem;          // this+Actor+0
    int       mAge;           // this+Actor+160
    int       mPickupDelay;   // this+Actor+164
    int       mThrowTime;     // this+Actor+168
    float     mBobOffs;       // this+Actor+172
    int       mHealth;        // this+Actor+176
    int       mLifeTime;      // this+Actor+180
    bool      mIsInItemFrame; // this+Actor+184
    bool      mIsFromFishing; // this+Actor+185
};