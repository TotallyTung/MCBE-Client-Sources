#pragma once


class InternalItemDescriptor
{
public:
    uintptr_t** vTable; // this+0x0

    // Recreate the getItem function using pseudo code above
    class Item* getItem() {
        if (this == nullptr) return nullptr;
        auto item = reinterpret_cast<Item**>(reinterpret_cast<uintptr_t>(this) + 0x8);
        if (!item || !*item) return nullptr;
        return *item;
    }

    Item** getRawItem() {
        return reinterpret_cast<Item**>(reinterpret_cast<uintptr_t>(this) + 0x8);
    }
}; 
//Size: 0x0010

class ItemDescriptor
{
public:
    uintptr_t** vTable;   // this+0x0
    InternalItemDescriptor* baseItem; // this+0x8
};
static_assert(sizeof(ItemDescriptor) == 0x10);

class ItemDescriptorCount : public ItemDescriptor
{
public:
    unsigned short mStackSize; // this+0x10
    char pad[0x6];
}; 
static_assert(sizeof(ItemDescriptorCount) == 0x18);


class NetworkItemStackDescriptor : public ItemDescriptorCount
{
public:
    char pad_0018[0x44]; //0x0018
    /*bool                  mIncludeNetIds{};  // this+0x18
    ItemStackNetIdVariant mNetIdVariant{};   // this+0x20
    unsigned int          mBlockRuntimeId{}; // this+0x38
    std::string           mUserDataBuffer{}; // this+0x40*/

    NetworkItemStackDescriptor() = default;
    NetworkItemStackDescriptor(class ItemStack const& itemStack)
    {
        memset(this, 0, sizeof(NetworkItemStackDescriptor));
        static void* func = reinterpret_cast<void*>(SigManager::NetworkStackItemDescriptor_ctor);
        Memory::CallFunc<void>(func, this, &itemStack);
    }
};
static_assert(sizeof(NetworkItemStackDescriptor) == 0x60);