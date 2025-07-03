//
// Created by vastrakai on 7/6/2024.
//

#include "SimpleContainer.hpp"

#include <SDK/OffsetProvider.hpp>
#include <Utils/MemUtils.hpp>

ItemStack* Container::getItem(int slot)
{
    static auto vFunc = OffsetProvider::Container_getItem;
    return MemUtils::callVirtualFunc<ItemStack*, int>(vFunc, this, slot);
}

void Container::setItem(int slot, ItemStack* item)
{
    static auto vFunc = OffsetProvider::Container_getItem + 5;
    MemUtils::callVirtualFunc<void, int, ItemStack*>(vFunc, this, slot, item);
}
