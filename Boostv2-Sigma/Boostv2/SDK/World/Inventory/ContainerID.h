#pragma once
#include <memory>

enum class ContainerID : int8_t {
    None = -1,
    Inventory = 0,
    First = 1,
    Last = 100,
    Offhand = 119,
    Armor = 120,
    SelectionSlots = 122,
    PlayerUIOnly = 124
};