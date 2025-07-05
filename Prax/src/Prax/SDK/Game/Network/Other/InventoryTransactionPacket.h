#pragma once

#include "src/Prax/SDK/Game/Network/Network.h"
#include <unordered_map>


/* Class: InventoryTransactionPacket
 * Purpose: Represents an inventory transaction packet
 * From: LeviLamina (LiteLoader)
 */

enum class InventorySourceType : int {
    InvalidInventory          = -1,
    ContainerInventory        = 0,
    GlobalInventory           = 1,
    WorldInteraction          = 2,
    CreativeInventory         = 3,
    NonImplementedFeatureTODO = 99999,
};

class InventorySource {
public:
    // InventorySource inner types define
    enum class InventorySourceFlags : unsigned int {
        NoFlag                  = 0x0,
        WorldInteraction_Random = 0x1,
    };

    InventorySourceType  mType        = InventorySourceType::InvalidInventory; // this+0x0
    char                 mContainerId = -1;                                     // this+0x4
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

class InternalItemDescriptor
{
public:
    uintptr_t** vTable; // this+0x0

    // Recreate the getItem function using pseudo code above
    Item* getItem() {
        if (this == nullptr) return nullptr;
        auto item = reinterpret_cast<Item**>(reinterpret_cast<uintptr_t>(this) + 0x8);
        if (!item || !*item) return nullptr;
        return *item;
    }

    Item** getRawItem() {
        return reinterpret_cast<Item**>(reinterpret_cast<uintptr_t>(this) + 0x8);
    }


}; //Size: 0x0010

class ItemDescriptor
{
public:
    uintptr_t** vTable; // this+0x0
    InternalItemDescriptor* baseItem; // this+0x8
}; //Size: 0x0010
static_assert(sizeof(ItemDescriptor) == 0x10);

class ItemDescriptorCount : public ItemDescriptor
{
public:
    unsigned short mStackSize; // this+0x10
    char pad[0x6];
}; //Size: 0x0018
static_assert(sizeof(ItemDescriptorCount) == 0x18);


class NetworkItemStackDescriptor : public ItemDescriptorCount
{
public:
    char pad_0018[72]; //0x0018
    /*bool                  mIncludeNetIds{};  // this+0x18
    ItemStackNetIdVariant mNetIdVariant{};   // this+0x20
    unsigned int          mBlockRuntimeId{}; // this+0x38
    std::string           mUserDataBuffer{}; // this+0x40*/

    NetworkItemStackDescriptor() = default;
    NetworkItemStackDescriptor(ItemStack const& itemStack)
    {
        memset(this, 0, sizeof(NetworkItemStackDescriptor));
        static void* func = reinterpret_cast<void*>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::NetworkStackItemDescriptor_ctor)));
        Memory::CallFastcall<void>(func, this, &itemStack);
    }
};

static_assert(sizeof(NetworkItemStackDescriptor) == 0x60);

class MobArmorEquipmentPacket : public ::Packet {
public:
    NetworkItemStackDescriptor mHead;  // this+0x30
    NetworkItemStackDescriptor mTorso; // this+0x90
    NetworkItemStackDescriptor mLegs;  // this+0xF0
    NetworkItemStackDescriptor mFeet;  // this+0x150

    uint64_t mRuntimeId; // this+0x1B0
};

class InventoryAction
{
public:
    InventorySource            mSource;             // this+0x0
    unsigned int               Slot;               // this+0xC
    NetworkItemStackDescriptor mFromItemDescriptor; // this+0x10
    NetworkItemStackDescriptor mToItemDescriptor;   // this+0x70
    ItemStack                  mFromItem;           // this+0xD0
    ItemStack                  mToItem;             // this+0x170

    InventoryAction(int slot, ItemStack* sourceItem, ItemStack* targetItem) {
        Slot = slot;
        if (sourceItem) mFromItem = *sourceItem;
        if (targetItem) mToItem = *targetItem;
        mSource = InventorySource();
        mSource.mType = InventorySourceType::ContainerInventory;
        if (sourceItem) mFromItemDescriptor = NetworkItemStackDescriptor(*sourceItem);
        if (targetItem) mToItemDescriptor = NetworkItemStackDescriptor(*targetItem);
    }
};

static_assert(sizeof(InventoryAction) == 0x210);

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
    std::unordered_map<InventorySource, std::vector<InventoryAction>> actions; // 0x0
    std::vector<InventoryTransactionItemGroup>                        items;   // 0x40

    void addAction(InventoryAction const& action) {
        static void* func = reinterpret_cast<void*>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::InventoryTransaction_addAction)));
        using AddAction = void(__fastcall*)(InventoryTransaction*, InventoryAction const&, bool);
        reinterpret_cast<AddAction>(func)(this, action, false);
    }

    std::string ToString() {
        std::string str = "";
        for (auto& action : actions) {
            for (auto& invAction : action.second) {
                ItemStack* fromItem = &invAction.mFromItem;
                ItemStack* toItem = &invAction.mToItem;
                std::string fromItemName = "air";
                std::string toItemName = "air";
                if (fromItem->item) {
                    Item* item = *fromItem->item;
                    fromItemName = item->GetName();
                }
                if (toItem->item) {
                    Item* item = *toItem->item;
                    toItemName = item->GetName();
                }
                str += "Slot: " + std::to_string(invAction.Slot) + " ";
                str += "FromItem: " + fromItemName + " ";
                str += "ToItem: " + toItemName + " ";
                str += "Source: " + std::to_string(static_cast<int>(action.first.mType)) + " ";
                str += "ContainerId: " + std::to_string(action.first.mContainerId) + " ";
                str += "Flags: " + std::to_string(static_cast<int>(action.first.mFlags)) + "\n";
            }
        }
        return str;
    }
};

enum TransacType : int {
    NormalTransaction          = 0x0,
    ItemUseTransaction         = 0x1,
    ItemUseOnEntityTransaction = 0x2,
    ItemReleaseTransaction     = 0x3,
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

    uintptr_t** vtable; // this+0x0
    Type                 type;
    InventoryTransaction data;

    ComplexInventoryTransaction()
    {
        static uintptr_t vtable_addr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ComplexInventoryTransaction_vtable), 3);
        vtable = reinterpret_cast<uintptr_t**>(vtable_addr);
        type = Type::NormalTransaction;
        data = InventoryTransaction();
        data.actions = std::unordered_map<InventorySource, std::vector<InventoryAction>>();
        data.items = std::vector<InventoryTransactionItemGroup>();
    };

    TransacType GetTransacType() {
        // Return the transaction type based on the vTable that is present
        static uintptr_t ComplexInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ComplexInventoryTransaction_vtable), 3);
        static uintptr_t ItemUseInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseInventoryTransaction_vtable), 3);
        static uintptr_t ItemUseOnActorInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseOnActorInventoryTransaction_vtable), 3);
        static uintptr_t ItemReleaseInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemReleaseInventoryTransaction_vtable), 3);

        if (vtable == reinterpret_cast<uintptr_t**>(ComplexInventoryTransaction_vTable)) return TransacType::NormalTransaction;
        if (vtable == reinterpret_cast<uintptr_t**>(ItemUseInventoryTransaction_vTable)) return TransacType::ItemUseTransaction;
        if (vtable == reinterpret_cast<uintptr_t**>(ItemUseOnActorInventoryTransaction_vTable)) return TransacType::ItemUseOnEntityTransaction;
        if (vtable == reinterpret_cast<uintptr_t**>(ItemReleaseInventoryTransaction_vTable)) return TransacType::ItemReleaseTransaction;
        return TransacType::NormalTransaction;
    };

    std::string GetTransactionType() {
        // Return the transaction type based on the vTable that is present
        static uintptr_t ComplexInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ComplexInventoryTransaction_vtable), 3);
        static uintptr_t ItemUseInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseInventoryTransaction_vtable), 3);
        static uintptr_t ItemUseOnActorInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseOnActorInventoryTransaction_vtable), 3);
        static uintptr_t ItemReleaseInventoryTransaction_vTable = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemReleaseInventoryTransaction_vtable), 3);

        if (vtable == reinterpret_cast<uintptr_t**>(ComplexInventoryTransaction_vTable)) return "NormalTransaction";
        if (vtable == reinterpret_cast<uintptr_t**>(ItemUseInventoryTransaction_vTable)) return "ItemUseTransaction";
        if (vtable == reinterpret_cast<uintptr_t**>(ItemUseOnActorInventoryTransaction_vTable)) return "ItemUseOnEntityTransaction";
        if (vtable == reinterpret_cast<uintptr_t**>(ItemReleaseInventoryTransaction_vTable)) return "ItemReleaseTransaction";
        return "UnknownTransaction";
    }
};

class ItemUseInventoryTransaction : public ComplexInventoryTransaction
{
public:
    enum class ActionType : int32_t {
        Place = 0,
        Use = 1,
        Destroy = 2,
    };

    ActionType actionType{};
    BlockPos blockPos{};
    uint32_t targetBlockRuntimeId{};
    uint8_t face{};
    int32_t slot{};
    NetworkItemStackDescriptor itemInHand{};
    Vector3 playerPos{}, clickPos{};

    ItemUseInventoryTransaction()
    {
        static uintptr_t vtable_addr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseInventoryTransaction_vtable), 3);
        vtable = reinterpret_cast<uintptr_t**>(vtable_addr);
        type = Type::ItemUseTransaction;
        data = InventoryTransaction();
        data.actions = std::unordered_map<InventorySource, std::vector<InventoryAction>>();
        data.items = std::vector<InventoryTransactionItemGroup>();
    }
};

class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction
{
public:
    enum class ActionType : int32_t {
        Interact = 0,
        Attack = 1,
        ItemInteract = 2,
    };

    uint64_t actorId{};
    ActionType actionType{};
    int32_t slot{};
    NetworkItemStackDescriptor itemInHand{};
    Vector3 playerPos{}, clickPos{};

    ItemUseOnActorInventoryTransaction()
    {
        static uintptr_t vtable_addr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemUseOnActorInventoryTransaction_vtable), 3);
        vtable = reinterpret_cast<uintptr_t**>(vtable_addr);
        type = Type::ItemUseOnEntityTransaction;
        data = InventoryTransaction();
        data.actions = std::unordered_map<InventorySource, std::vector<InventoryAction>>();
        data.items = std::vector<InventoryTransactionItemGroup>();
    }
};

class ItemReleaseInventoryTransaction : public ComplexInventoryTransaction
{
public:
    enum class ActionType : int32_t {
        Release = 0,
        Use = 1,
    };

    ActionType actionType{};
    int32_t slot{};
    NetworkItemStackDescriptor itemInHand{};
    Vector3 headPos{};

    ItemReleaseInventoryTransaction()
    {
        static uintptr_t vtable_addr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemReleaseInventoryTransaction_vtable), 3);
        vtable = reinterpret_cast<uintptr_t**>(vtable_addr);
        type = Type::ItemReleaseTransaction;
        data = InventoryTransaction();
        data.actions = std::unordered_map<InventorySource, std::vector<InventoryAction>>();
        data.items = std::vector<InventoryTransactionItemGroup>();
    }
};

static_assert(sizeof(ComplexInventoryTransaction) == 104);
static_assert(sizeof(ItemUseInventoryTransaction) == 256);
static_assert(sizeof(ItemUseOnActorInventoryTransaction) == 240);
static_assert(sizeof(ItemReleaseInventoryTransaction) == 224);


enum class ContainerEnumName : signed char {
    AnvilInputContainer                 = 0x0,
    AnvilMaterialContainer              = 0x1,
    AnvilResultPreviewContainer         = 0x2,
    SmithingTableInputContainer         = 0x3,
    SmithingTableMaterialContainer      = 0x4,
    SmithingTableResultPreviewContainer = 0x5,
    ArmorContainer                      = 0x6,
    LevelEntityContainer                = 0x7,
    BeaconPaymentContainer              = 0x8,
    BrewingStandInputContainer          = 0x9,
    BrewingStandResultContainer         = 0xA,
    BrewingStandFuelContainer           = 0xB,
    CombinedHotbarAndInventoryContainer = 0xC,
    CraftingInputContainer              = 0xD,
    CraftingOutputPreviewContainer      = 0xE,
    RecipeConstructionContainer         = 0xF,
    RecipeNatureContainer               = 0x10,
    RecipeItemsContainer                = 0x11,
    RecipeSearchContainer               = 0x12,
    RecipeSearchBarContainer            = 0x13,
    RecipeEquipmentContainer            = 0x14,
    RecipeBookContainer                 = 0x15,
    EnchantingInputContainer            = 0x16,
    EnchantingMaterialContainer         = 0x17,
    FurnaceFuelContainer                = 0x18,
    FurnaceIngredientContainer          = 0x19,
    FurnaceResultContainer              = 0x1A,
    HorseEquipContainer                 = 0x1B,
    HotbarContainer                     = 0x1C,
    InventoryContainer                  = 0x1D,
    ShulkerBoxContainer                 = 0x1E,
    TradeIngredient1Container           = 0x1F,
    TradeIngredient2Container           = 0x20,
    TradeResultPreviewContainer         = 0x21,
    OffhandContainer                    = 0x22,
    CompoundCreatorInput                = 0x23,
    CompoundCreatorOutputPreview        = 0x24,
    ElementConstructorOutputPreview     = 0x25,
    MaterialReducerInput                = 0x26,
    MaterialReducerOutput               = 0x27,
    LabTableInput                       = 0x28,
    LoomInputContainer                  = 0x29,
    LoomDyeContainer                    = 0x2A,
    LoomMaterialContainer               = 0x2B,
    LoomResultPreviewContainer          = 0x2C,
    BlastFurnaceIngredientContainer     = 0x2D,
    SmokerIngredientContainer           = 0x2E,
    Trade2Ingredient1Container          = 0x2F,
    Trade2Ingredient2Container          = 0x30,
    Trade2ResultPreviewContainer        = 0x31,
    GrindstoneInputContainer            = 0x32,
    GrindstoneAdditionalContainer       = 0x33,
    GrindstoneResultPreviewContainer    = 0x34,
    StonecutterInputContainer           = 0x35,
    StonecutterResultPreviewContainer   = 0x36,
    CartographyInputContainer           = 0x37,
    CartographyAdditionalContainer      = 0x38,
    CartographyResultPreviewContainer   = 0x39,
    BarrelContainer                     = 0x3A,
    CursorContainer                     = 0x3B,
    CreatedOutputContainer              = 0x3C,
    SmithingTableTemplateContainer      = 0x3D,
};



class InventoryTransactionPacket : public Packet
{
public:
    static const PacketID ID = PacketID::InventoryTransaction;

    char pad[0x10]; // this+0x30
    std::vector<std::pair<ContainerEnumName, std::vector<unsigned char>>> mLegacySetItemSlots;   // this+0x40
    std::unique_ptr<ComplexInventoryTransaction>                          mTransaction;        // this+0x58
    //ComplexInventoryTransaction* mTransaction;                                                   // this+0x58
    bool                                                                  mIsClientSide = false; // this+0x60
};

static_assert(sizeof(InventoryTransactionPacket) == 0x68);