#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <Utils/MemUtils.hpp>

#include "SimpleContainer.hpp"

class Inventory : public SimpleContainer {
public:
    void dropSlot(int slot);
    void swapSlots(int from, int to);
    void equipArmor(int slot);
    void startUsingItem(int slot);
    void releaseUsingItem(int slot);
};

class PlayerInventory {
public:
    CLASS_FIELD(int, mSelectedSlot, 0x10);
    CLASS_FIELD(int, mInHandSlot, 0x1C);

    Inventory* getContainer();
};