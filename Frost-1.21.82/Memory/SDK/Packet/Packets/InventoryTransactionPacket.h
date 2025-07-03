#pragma once

struct ItemStackLegacyRequestIdTag {
public:
};

struct ItemStackNetIdTag {
public:
};

struct ItemStackRequestIdTag {
public:
};

using ItemStackLegacyRequestId = unsigned int;
using ItemStackNetId = unsigned int;
using ItemStackRequestId = unsigned int;

struct ItemStackNetIdVariant
{
public:
    std::variant<ItemStackNetId, ItemStackRequestId, ItemStackLegacyRequestId> id;
};

class InventorySource {
public:
    // InventorySource inner types define
    enum class InventorySourceFlags : unsigned int {
        NoFlag                  = 0x0,
        WorldInteraction_Random = 0x1,
    };

    InventorySourceType  mType        = InventorySourceType::InvalidInventory; // this+0x0
    ContainerID          mContainerId = ContainerID::None;                     // this+0x4
    InventorySourceFlags mFlags       = InventorySourceFlags::NoFlag;          // this+0x8

    inline bool operator==(InventorySource const& rhs) const {
        return ((this->mType == rhs.mType) &&
            (this->mContainerId == rhs.mContainerId) &&
            (this->mFlags == rhs.mFlags));
    }
    inline bool operator!=(InventorySource const& rhs) const {
        return !(*this == rhs);
    }

    inline bool operator<(InventorySource const& rhs) const {
        if (this->mType != rhs.mType) {
            return this->mType < rhs.mType;
        }
        if (this->mContainerId != rhs.mContainerId) {
            return this->mContainerId < rhs.mContainerId;
        }
        return this->mFlags < rhs.mFlags;
    }

    inline bool operator>(InventorySource const& rhs) const {
        return rhs < *this;
    }

};

namespace std
{
    template <>
    struct hash<InventorySource> {
        size_t operator()(InventorySource const& key) const {
            return static_cast<size_t>(key.mContainerId)
                ^ (static_cast<size_t>(static_cast<unsigned int>(key.mType)) << 16);
        }
    };
}

class InventoryAction
{
public:
    InventorySource            mSource;             // this+0x0
    unsigned int               mSlot;               // this+0xC
    NetworkItemStackDescriptor mFromItemDescriptor; // this+0x10
    NetworkItemStackDescriptor mToItemDescriptor;   // this+0x70
    ItemStack                  mFromItem;           // this+0xD0
    ItemStack                  mToItem;             // this+0x170

    InventoryAction(int slot, ItemStack* sourceItem, ItemStack* targetItem) {
        mSlot = slot;
        //if (sourceItem) mFromItem = *sourceItem;
        //if (targetItem) mToItem = *targetItem;
        mSource = InventorySource();
        mSource.mType = InventorySourceType::ContainerInventory;
        if (sourceItem) mFromItemDescriptor = NetworkItemStackDescriptor(*sourceItem);
        if (targetItem) mToItemDescriptor = NetworkItemStackDescriptor(*targetItem);
    }
};
//static_assert(sizeof(InventoryAction) == 0x200);

class InventoryTransactionItemGroup {
public:
    InventoryTransactionItemGroup() = delete;

    int                                mItemId;   // this+0x0
    int                                mItemAux;  // this+0x4
    std::unique_ptr<class CompoundTag> mTag;      // this+0x8
    int                                mCount;    // this+0x10
    bool                               mOverflow; // this+0x14
};

class InventoryTransaction {
public:
    std::unordered_map<InventorySource, std::vector<InventoryAction>> mActions; // this+0x0
    std::vector<InventoryTransactionItemGroup>                        mItems;   // this+0x40

    void addAction(InventoryAction const& action) {
        static void* func = reinterpret_cast<void*>(SigManager::InventoryTransaction_addAction);
        using AddAction = void(__fastcall*)(InventoryTransaction*, InventoryAction const&, bool);
        reinterpret_cast<AddAction>(func)(this, action, false);
    }
};

class ComplexInventoryTransaction {
public:
    // ComplexInventoryTransaction inner types define
    enum class Type : int {
        NormalTransaction          = 0x0,
        InventoryMismatch          = 0x1,
        ItemUseTransaction         = 0x2,
        ItemUseOnEntityTransaction = 0x3,
        ItemReleaseTransaction     = 0x4,
    };

    uintptr_t**          Vtable; // this+0x0

    Type                 mType;   
    InventoryTransaction mData;

    Type getTransactionsType() {
        return mType;
    };
};

class ItemUseInventoryTransaction : public ComplexInventoryTransaction
{
public:
    // ItemUseInventoryTransaction inner types define
    enum class ActionType : int32_t {
        Place   = 0,
        Use     = 1,
        Destroy = 2,
    };

    enum class PredictedResult : int32_t {
        Failure = 0,
        Success = 1
    };

    enum class TriggerType : int8_t {
        Unknown        =  0,
        PlayerInput    = 1,
        SimulationTick = 2,
    };

    ActionType                 mActionType{};                               // this+0x0
    TriggerType                mTriggerType = TriggerType::PlayerInput;     // this+0x4
    Vector3<int>               mBlockPos{};                                 // this+0x8
    int32_t                    mTargetBlockRuntimeId{};                     // this+0x14
    uint32_t                   mFace{};                                     // this+0x18
    uint32_t                   mSlot{};                                     // this+0x1C
    NetworkItemStackDescriptor mItemInHand{};                               // this+0x20
    Vector3<float>             mPlayerPos{};                                // this+0x30
    Vector3<float>             mClickPos{};                                 // this+0x3C
    PredictedResult            mPredictedResult = PredictedResult::Success; // this+0x48
};

class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction
{
public:
    // ItemUseOnActorInventoryTransaction inner types define
    enum class ActionType : int32_t {
        Interact     = 0,
        Attack       = 1,
        ItemInteract = 2,
    };

    uint64_t                   mActorId{};    // this+0x00
    ActionType                 mActionType{}; // this+0x08
    int32_t                    mSlot{};       // this+0x0C
    NetworkItemStackDescriptor mItemInHand{}; // this+0x10
    Vector3<float>             mPlayerPos{};  // this+0x20
    Vector3<float>             mClickPos{};   // this+0x2C
};

class ItemReleaseInventoryTransaction : public ComplexInventoryTransaction
{
public:
    // ItemReleaseInventoryTransaction inner types define
    enum class ActionType : int32_t {
        Release = 0,
        Use     = 1,
    };

    ActionType                 mActionType{}; // this+0x00
    int32_t                    mSlot{};       // this+0x04
    NetworkItemStackDescriptor mItemInHand{}; // this+0x08
    Vector3<float>             mHeadPos{};    // this+0x18
};

//static_assert(sizeof(ComplexInventoryTransaction) == 104);
//static_assert(sizeof(ItemUseInventoryTransaction) == 264); // 256
//static_assert(sizeof(ItemUseOnActorInventoryTransaction) == 240);
//static_assert(sizeof(ItemReleaseInventoryTransaction) == 224);

class InventoryTransactionPacket : public Packet
{
public:
    char                                                                   pad[0x10];             // this+0x30
    std::vector<std::pair<ContainerEnumName, std::vector<unsigned char>>>  mLegacySetItemSlots;   // this+0x40
    std::unique_ptr<ComplexInventoryTransaction>                           mTransaction;          // this+0x58
    bool                                                                   mIsClientSide = false; // this+0x60
};

//static_assert(sizeof(InventoryTransactionPacket) == 0x68);