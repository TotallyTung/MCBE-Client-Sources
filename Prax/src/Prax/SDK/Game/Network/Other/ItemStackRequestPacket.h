//
// Created by vastrakai on 10/13/2023.
//


#include <vector>
#include "src/Prax/SDK/Game/Network/Network.h"
//
//enum class ContainerEnumName : int8_t {
//    AnvilInputContainer = 0,
//    AnvilMaterialContainer = 1,
//    AnvilResultPreviewContainer = 2,
//    SmithingTableInputContainer = 3,
//    SmithingTableMaterialContainer = 4,
//    SmithingTableResultPreviewContainer = 5,
//    ArmorContainer = 6,
//    LevelEntityContainer = 7,
//    BeaconPaymentContainer = 8,
//    BrewingStandInputContainer = 9,
//    BrewingStandResultContainer = 10,
//    BrewingStandFuelContainer = 11,
//    CombinedHotbarAndInventoryContainer = 12,
//    CraftingInputContainer = 13,
//    CraftingOutputPreviewContainer = 14,
//    RecipeConstructionContainer = 15,
//    RecipeNatureContainer = 16,
//    RecipeItemsContainer = 17,
//    RecipeSearchContainer = 18,
//    RecipeSearchBarContainer = 19,
//    RecipeEquipmentContainer = 20,
//    RecipeBookContainer = 21,
//    EnchantingInputContainer = 22,
//    EnchantingMaterialContainer = 23,
//    FurnaceFuelContainer = 24,
//    FurnaceIngredientContainer = 25,
//    FurnaceResultContainer = 26,
//    HorseEquipContainer = 27,
//    HotbarContainer = 28,
//    InventoryContainer = 29,
//    ShulkerBoxContainer = 30,
//    TradeIngredient1Container = 31,
//    TradeIngredient2Container = 32,
//    TradeResultPreviewContainer = 33,
//    OffhandContainer = 34,
//    CompoundCreatorInput = 35,
//    CompoundCreatorOutputPreview = 36,
//    ElementConstructorOutputPreview = 37,
//    MaterialReducerInput = 38,
//    MaterialReducerOutput = 39,
//    LabTableInput = 40,
//    LoomInputContainer = 41,
//    LoomDyeContainer = 42,
//    LoomMaterialContainer = 43,
//    LoomResultPreviewContainer = 44,
//    BlastFurnaceIngredientContainer = 45,
//    SmokerIngredientContainer = 46,
//    Trade2Ingredient1Container = 47,
//    Trade2Ingredient2Container = 48,
//    Trade2ResultPreviewContainer = 49,
//    GrindstoneInputContainer = 50,
//    GrindstoneAdditionalContainer = 51,
//    GrindstoneResultPreviewContainer = 52,
//    StonecutterInputContainer = 53,
//    StonecutterResultPreviewContainer = 54,
//    CartographyInputContainer = 55,
//    CartographyAdditionalContainer = 56,
//    CartographyResultPreviewContainer = 57,
//    BarrelContainer = 58,
//    CursorContainer = 59,
//    CreatedOutputContainer = 60
//};
//
//class ItemStackNetIdVariant {
//public:
//    int id1;
//private:
//    char pad1[4];
//public:
//    int id2;
//private:
//    char pad2[4];
//public:
//    uint8_t type;
//private:
//    char pad3[7];
//public:
//    ItemStackNetIdVariant() {
//        memset(this, 0, sizeof(ItemStackNetIdVariant));
//    }
//
//    __forceinline int getID() {
//        if (type == 0) return id1;
//        else if (type <= 2) return id2;
//        else return 0;
//    }
//
//    __forceinline void setID(int id) {
//        if (type == 0) id1 = id;
//        else if (type <= 2) id2 = id;
//    }
//
//    ItemStackNetIdVariant(int id) {
//        memset(this, 0, sizeof(ItemStackNetIdVariant));
//        setID(id);
//    }
//};
//
//class ItemStackRequestSlotInfo {
//public:
//    ContainerEnumName containerEnumName;
//    uint8_t slot;
//private:
//    char pad[6];
//public:
//    ItemStackNetIdVariant networkStackID;
//
//    ItemStackRequestSlotInfo() {
//        memset(this, 0, sizeof(ItemStackRequestSlotInfo));
//    }
//
//    ItemStackRequestSlotInfo(ContainerEnumName containerEnumName, uint8_t slot, ItemStackNetIdVariant networkStackID) {
//        memset(this, 0, sizeof(ItemStackRequestSlotInfo));
//        this->containerEnumName = containerEnumName;
//        this->slot = slot;
//        this->networkStackID = networkStackID;
//    }
//};
//
//enum ItemStackRequestActionType : uint8_t {
//    Take = 0x0,
//    Place = 0x1,
//    Swap = 0x2,
//    Drop = 0x3,
//    Destroy = 0x4,
//    Consume = 0x5,
//    Create = 0x6,
//    PlaceInItemContainer = 0x7,
//    TakeFromItemContainer = 0x8,
//    ScreenLabTableCombine = 0x9,
//    ScreenBeaconPayment = 0xA,
//    ScreenHUDMineBlock = 0xB,
//    CraftRecipe = 0xC,
//    CraftRecipeAuto = 0xD,
//    CraftCreative = 0xE,
//    CraftRecipeOptional = 0xF,
//    CraftRepairAndDisenchant = 0x10,
//    CraftLoom = 0x11,
//    CraftNonImplemented_DEPRECATEDASKTYLAING = 0x12,
//    CraftResults_DEPRECATEDASKTYLAING = 0x13,
//    OnRender = 0x14,
//};
//
//class ItemStackRequestAction {
//public:
//    uintptr_t vtable;
//    ItemStackRequestActionType type;
//
//private:
//    char pad_cockandballs[7];
//};
//
//class ItemStackRequestActionTransferBase : public ::ItemStackRequestAction {};
//
//class ItemStackRequestActionSwap : public ItemStackRequestActionTransferBase {
//private:
//    uint64_t one;
//public:
//    ItemStackRequestSlotInfo slot1;
//    ItemStackRequestSlotInfo slot2;
//
//    ItemStackRequestActionSwap(const ItemStackRequestSlotInfo& s1, const ItemStackRequestSlotInfo& s2) {
//        static uintptr_t vt = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestAction_swap), 3);
//        memset(this, 0x0, sizeof(ItemStackRequestActionSwap));
//        this->vtable = vt;
//        this->type = ItemStackRequestActionType::Swap;
//        this->one = 1;
//        this->slot1 = s1;
//        this->slot2 = s2;
//    }
//};
//
//class ItemStackRequestActionDrop : public ItemStackRequestActionTransferBase {
//private:
//    char pad[0x57];
//public:
//
//    ItemStackRequestActionDrop(uint8_t amount, bool randomDropLocation, const ItemStackRequestSlotInfo& slotInfo) {
//        memset(this, 0x0, sizeof(ItemStackRequestActionDrop));
//        using dropAction_ctor = void(__fastcall*)(ItemStackRequestActionDrop*, uint8_t, bool, const ItemStackRequestSlotInfo&);
//        static dropAction_ctor ctor = reinterpret_cast<dropAction_ctor>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestAction_drop)));
//        ctor(this, amount, randomDropLocation, slotInfo);
//    }
//};
//
//
//
//struct ItemStackRequestData {
//    uintptr_t vtable;
//    uint32_t weirdCounterThingy;
//    uint32_t zero;
//    uintptr_t zeroes[3];
//    uintptr_t thingy;
//    std::vector<ItemStackRequestAction*> actions;
//
//    ItemStackRequestData(std::vector<ItemStackRequestAction*> actions) {
//        static uintptr_t vt = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestData), 3);
//        this->vtable = vt;
//        this->zero = 0;
//        memset(this->zeroes, 0, 24);
//        this->weirdCounterThingy = 0xFFFFFFFD;
//        this->thingy = 0x00000000FFFFFFFF;
//        this->actions = actions;
//    }
//};
//
//class ItemStackRequestBatch {
//public:
//    std::vector<ItemStackRequestData *> requests;
//
//    ItemStackRequestBatch(std::vector<ItemStackRequestData *> requests) {
//        this->requests = requests;
//    }
//};
//
//class ItemStackRequestPacket : public Packet {
//private:
//    char __paddingA[40];
//public:
//    ItemStackRequestBatch* requestBatch;
//
//public:
//    ItemStackRequestPacket(ItemStackRequestBatch* requestBatch) {
//        this->requestBatch = requestBatch;
//    };
//
//    ItemStackRequestPacket() = default;
//};