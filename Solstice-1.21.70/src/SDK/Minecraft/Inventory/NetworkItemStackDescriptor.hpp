#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <cstdint>
#include <string>
#include <variant>
#include <SDK/SigManager.hpp>
#include <SDK/Minecraft/JSON.hpp>
#include <Utils/MemUtils.hpp>


struct ItemStackLegacyRequestIdTag {
public:
};

struct ItemStackNetIdTag {
public:
};

struct ItemStackRequestIdTag {
public:
};

struct ItemTag : public HashedString {};

class ItemDescriptor {
public:
    enum class InternalType : signed char {
        Invalid      = 0x0,
        Default      = 0x1,
        Molang       = 0x2,
        ItemTag      = 0x3,
        Deferred     = 0x4,
        ComplexAlias = 0x5,
    };

    struct ItemEntry {
    public:
        const class Item* mItem;     // this+0x0
        short             mAuxValue; // this+0x8
    };

    struct BaseDescriptor {
    public:
        Item* mItem; // this+0x0
        //ItemTag mItemTag;

        BaseDescriptor() = default;
        BaseDescriptor(BaseDescriptor& self)
        {
            *reinterpret_cast<uintptr_t**>(this) = *reinterpret_cast<uintptr_t**>(&self);
            mItem = self.mItem;
        }


    public:
        virtual std::unique_ptr<struct ItemDescriptor::BaseDescriptor> clone() const { return nullptr; }
        virtual bool sameItems(struct ItemDescriptor::BaseDescriptor const& otherDescriptor, bool compareAux) const { return false; }
        virtual bool sameItem(struct ItemDescriptor::ItemEntry const& otherItem, bool) const { return false; }
        virtual std::string const& getFullName() const { return std::string(); }
        virtual struct ItemDescriptor::ItemEntry getItem() const { return ItemEntry(); }
        virtual bool forEachItemUntil(std::function<bool(class Item const&, short)> func) const { return false; }
        virtual std::map<std::string, std::string> toMap() const { return std::map<std::string, std::string>(); }
        virtual void /*std::optional<class CompoundTag>*/ save() const { return; }
        virtual void serialize(class MinecraftJson::Value& val) const { return; }
        virtual void serialize(class BinaryStream& stream) const {}
        virtual ::ItemDescriptor::InternalType getType() const { return ::ItemDescriptor::InternalType::Invalid; }
        virtual bool isValid() const { return false; }
        virtual uint64_t getHash() const { return 0; }
        virtual bool shouldResolve() const { return false; }
        virtual std::unique_ptr<struct ItemDescriptor::BaseDescriptor> resolve() const { return nullptr; }
        virtual ~BaseDescriptor() = default;


    };

    uintptr_t** vtable;
    ItemDescriptor::BaseDescriptor* mImpl;
};
static_assert(sizeof(ItemDescriptor) == 0x10);

class ItemDescriptorCount : public ItemDescriptor
{
public:
    unsigned short mStackSize; // this+0x10
    PAD(0x6);
}; //Size: 0x0018
static_assert(sizeof(ItemDescriptorCount) == 0x18);


class NetworkItemStackDescriptor : public ItemDescriptorCount
{
public:
    PAD(0x44);
    /*bool                  mIncludeNetIds{};  // this+0x18
    ItemStackNetIdVariant mNetIdVariant{};   // this+0x20
    unsigned int          mBlockRuntimeId{}; // this+0x38
    std::string           mUserDataBuffer{}; // this+0x40*/

    NetworkItemStackDescriptor() = default;
    NetworkItemStackDescriptor(class ItemStack const& itemStack)
    {
        memset(this, 0, sizeof(NetworkItemStackDescriptor));
        static void* func = reinterpret_cast<void*>(SigManager::NetworkStackItemDescriptor_ctor);
        MemUtils::callFastcall<void>(func, this, &itemStack);
    }
};

static_assert(sizeof(NetworkItemStackDescriptor) == 0x60);