#pragma once

#include <Prax/SDK/SigManager.h>
#include <Prax/Utils/System/Logger.h>
#include <Prax/Utils/System/Memory.h>

#include <Prax/SDK/Game/Misc/Text.h>
#include <Prax/SDK/Game/Misc/Structs.h>

#include <memory>
#include <string>
#include <cstdint>
#include <xorstr.hpp>

class ItemStackBase {
public:
    uintptr_t** vtable;
    class Item** item;
    void* compoundTag;
    class Block* blockPtr;
    short auxValue;
    int8_t amount;
private:
    char pad_0x1B[0x60];
};

class InventoryTransactionManager
{
public:
    void addAction(const class InventoryAction& action);
};


enum class Enchant : int {
    PROTECTION = 0,
    FIRE_PROTECTION = 1,
    FEATHER_FALLING = 2,
    BLAST_PROTECTION = 3,
    PROJECTILE_PROTECTION = 4,
    THORNS = 5,
    RESPIRATION = 6,
    DEPTH_STRIDER = 7,
    AQUA_AFFINITY = 8,
    SHARPNESS = 9,
    SMITE = 10,
    BANE_OF_ARTHROPODS = 11,
    KNOCKBACK = 12,
    FIRE_ASPECT = 13,
    LOOTING = 14,
    EFFICIENCY = 15,
    SILK_TOUCH = 16,
    UNBREAKING = 17,
    FORTUNE = 18,
    POWER = 19,
    PUNCH = 20,
    FLAME = 21,
    BOW_INFINITY = 22,
    LUCK_OF_THE_SEA = 23,
    LURE = 24,
    FROST_WALKER = 25,
    MENDING = 26,
    BINDING = 27,
    VANISHING = 28,
    IMPALING = 29,
    RIPTIDE = 30,
    LOYALTY = 31,
    CHANNELING = 32,
    MULTISHOT = 33,
    PIERCING = 34,
    QUICK_CHARGE = 35,
    SOUL_SPEED = 36,
    SWIFT_SNEAK = 37
};

class ItemStack : public ItemStackBase {
public:
    uint8_t stackNetId = 0; //0x0088
    char pad_0089[23]; //0x0089

    static ItemStack* getEmptyItem()
    {
        static uint64_t what = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStack_EMPTY_ITEM), 3);
        static ItemStack* emptyItem = reinterpret_cast<ItemStack*>(what);
        return emptyItem;
    }

    void fromDescriptor(class NetworkItemStackDescriptor const& desc);

    std::string GetCustomName() {
        std::string name;
        //MemoryUtil::CallFunc<std::string*, ItemStack*>(Lunaris::GetSigManager()->GetAddressOf(SigMap::ItemStackBase_getName), this, &name);
        static uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStack_getCustomName));
        static auto _ItemStackBase_getName = (std::string*(__fastcall*)(ItemStack*, std::string*))func;
        _ItemStackBase_getName(this, &name);
        return name;
    }

    int getEnchantValue(int id) {
        using getEnchantsLevel_t = int(__fastcall*)(int, ItemStack*);
        static getEnchantsLevel_t getEnchantsLevel =(getEnchantsLevel_t)(SigManager::GetSignatureAddress(SignatureEnum::EnchantUtils_getEnchantLevel));
        return getEnchantsLevel(id, this);
    }

    int getEnchantValue(Enchant ench) {
        return getEnchantValue((int)ench);
    }

    ItemStack()
    {
        static uintptr_t result = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStack_vtable), 3);
        vtable = reinterpret_cast<uintptr_t**>(result);
    };
};

static_assert(sizeof(ItemStack) == 0xA0);

class Tag
{
public:
    enum class Type : uint8_t {
        End,
        Byte,
        Short,
        Int,
        Int64,
        Float,
        Double,
        ByteArray,
        String,
        List,
        Compound,
        IntArray,
    };

    virtual ~Tag();
    virtual void deleteChildren();
    virtual void Func2();
    virtual void load(class IDataInput&);
    virtual std::string toString();
    virtual Type getId();
    virtual bool equals(const Tag&) const;
    virtual void print(class PrintStream&) const;
    virtual void print(std::string const&, class PrintStream&) const;
    virtual std::unique_ptr<Tag> copy() const = 0;
    virtual void Func10();
};

class CompoundTagVariant
{
    char pad_0000[40]; //0x0000
    Tag::Type type; //0x0028
    char pad_002C[7]; //0x002C
public:
    inline Tag::Type getTagType() {
        return this->type;
    }
    class StringTag : public Tag
    {
    public:
        std::string val;
    };
    inline StringTag* asStringTag() {
        return (StringTag*)this;
    }
};

class CompoundTag {
public:
    std::map<std::string, CompoundTagVariant> data;

    virtual ~CompoundTag();
    virtual void deleteChildren(void);
    virtual void write(class IDataOutput&);
    virtual void load(class IDataInput&);
    virtual std::string toString(void);
    virtual void getId(void);
    virtual void equals(class Tag const&);
    virtual void print(class PrintStream&);
    virtual void print(std::string const&, class PrintStream&);
    virtual void copy(void);
    virtual void hash(void);
};

class Container {
public:
    // new vTable
    virtual ~Container();
    virtual void init(void);
    virtual void addContentChangeListener(class ContainerContentChangeListener*);
    virtual void removeContentChangeListener(ContainerContentChangeListener*);
    virtual void addRemovedListener(class ContainerRemovedListener*);
    virtual void removeRemovedListener(ContainerRemovedListener*);
    virtual void Function5();
    virtual class ItemStack* getItem(int);
    virtual bool hasRoomForItem(ItemStack const&);
    virtual void addItem(ItemStack&);
    virtual void addItemWithForceBalance(ItemStack&);
    virtual void addItemToFirstEmptySlot(ItemStack const&);
    virtual int Function8();
    virtual void setItemWithForceBalance(int, ItemStack const&, bool);
    virtual void removeItem(int, int);
    virtual void removeAllItems(void);
    virtual void removeAllItemsWithForceBalance(void);
    virtual void containerRemoved(void);
    virtual void dropSlotContent(class BlockSource&, Vector3 const&, bool, int);
    virtual void dropContents(BlockSource&, Vector3 const&, bool);
    virtual void getContainerSize(void);
    virtual void getMaxStackSize(void);
    virtual void startOpen(class Actor&);
    virtual void stopOpen(Actor&);
    virtual void getSlotCopies(void);
    virtual void getSlots(void);
    virtual void getEmptySlotsCount(void);
    virtual void getItemCount(ItemStack const&);
    virtual void findFirstSlotForItem(ItemStack const&);
    virtual bool canPushInItem(int, int, ItemStack const&);
    virtual bool canPullOutItem(int, int, ItemStack const&);
    virtual void setContainerChanged(int);
    virtual void setContainerMoved(void);
    virtual void setCustomName(std::string const&);
    virtual bool hasCustomName(void);
    virtual void readAdditionalSaveData(class CompoundTag const&);
    virtual void addAdditionalSaveData(CompoundTag&);
    virtual void initializeContainerContents(class BlockSource&);
    virtual bool isEmpty(void);
    virtual bool isSlotDisabled(int);

};

class SimpleContainer : public Container {};


class Item {
public:
    BUILD_ACCESS(this, uintptr_t**, vtable, 0x0);
    BUILD_ACCESS(this, short, id, 0xA2);
    BUILD_ACCESS(this, int, armorItemType, 0x244);
    BUILD_ACCESS(this, int, protectionAmount, 0x25C);

    // new vTable
    virtual ~Item();
    virtual void initServer(/*Json::Value&, SemVersion const&, bool, Experiments const&*/);
    virtual void tearDown(void);
    virtual void setDescriptionId(std::string const&);
    virtual void getDescriptionId(void);
    virtual void getMaxUseDuration(ItemStack const*);
    virtual bool isMusicDisk(void);
    virtual void executeEvent(class ItemStackBase&, std::string const&, class RenderParams&);
    virtual bool isComponentBased(void);
    virtual bool isHumanoidArmor(void);
    virtual bool isBlockPlanterItem(void);
    virtual bool isBucket(void);
    virtual bool isCandle(void);
    virtual bool isDamageable(void);
    virtual bool isDyeable(void);
    virtual bool isDye(void);
    virtual void getItemColor(void);
    virtual bool isFertilizer(void);
    virtual bool isFood(void);
    virtual bool isThrowable(void);
    virtual bool isUseable(void);
    virtual bool isTrimAllowed(void);
    virtual void getComponent(class HashedString const&);
    virtual void getFood(void);
    virtual void setMaxDamage(int);
    virtual void setMaxUseDuration(int);
    virtual void buildNetworkTag(void);
    virtual void initializeFromNetwork(CompoundTag const&);
    virtual void validateFromNetwork(CompoundTag const&);
    virtual void getBlockShape(void);
    virtual bool canBeDepleted(void);
    virtual bool canDestroySpecial(Block const&);
    virtual void getLevelDataForAuxValue(int);
    virtual bool isStackedByData(void);
    virtual void getMaxDamage(void);
    virtual void getAttackDamage(void);
    virtual void getAttackDamageBonus(class Actor const&, float);
    virtual bool isHandEquipped(void);
    virtual bool isGlint(ItemStackBase const&);
    virtual bool isPattern(void);
    virtual void getPatternIndex(void);
    virtual void showsDurabilityInCreative(void);
    virtual bool isWearableThroughLootTable(CompoundTag const*);
    virtual bool canDestroyInCreative(void);
    virtual bool isDestructive(int);
    virtual bool isLiquidClipItem(int);
    virtual void shouldInteractionWithBlockBypassLiquid(Block const&);
    virtual void requiresInteract(void);
    virtual void appendFormattedHovertext(ItemStackBase const&, class Level&, std::string&, bool);
    virtual bool isValidRepairItem(ItemStackBase const&, ItemStackBase const&, class BaseGameVersion const&);
    virtual void getEnchantSlot(void);
    virtual int getEnchantValue(void);
    virtual int getArmorValue(void);
    virtual float getToughnessValue(void);
    virtual void getBreakSound(void);
    virtual bool isComplex(void);
    virtual bool isValidAuxValue(int);
    virtual void getDamageChance(int);
    virtual void getViewDamping(void);
    virtual void uniqueAuxValues(void);
    virtual bool isActorPlacerItem(void);
    virtual bool isMultiColorTinted(ItemStack const&);
    virtual void getColor(CompoundTag const*, class ItemDescriptor const&);
    virtual bool hasCustomColor(ItemStackBase const&);
    virtual bool hasCustomColor(CompoundTag const*);
    virtual void clearColor(ItemStackBase&);
    virtual void setColor(ItemStackBase&, rgb const&); // (mce::color)
    virtual void getBaseColor(ItemStack const&);
    virtual void getSecondaryColor(ItemStack const&);
    virtual void getActorIdentifier(ItemStack const&);
    virtual void buildIdAux(short, CompoundTag const*);
    virtual bool canUseOnSimTick(void);
    virtual void use(ItemStack&, class Actor&);
    virtual void createProjectileActor(BlockSource&, ItemStack const&, Vector3 const&, Vector3 const&);
    virtual void dispense(BlockSource&, Container&, int, Vector3 const&, unsigned char);
    virtual void useTimeDepleted(ItemStack&, Level*, Actor*);
    virtual void releaseUsing(ItemStack&, Actor*, int);
    virtual float getDestroySpeed(ItemStack*, Block const&);
    virtual void hurtActor(ItemStack&, Actor&, Actor&);
    virtual void hitActor(ItemStack&, Actor&, Actor&);
    virtual void hitBlock(ItemStack&, Block const&, BlockPos const&, Actor&);
    virtual void mineBlock(ItemStack&, Block const&, int, int, int, Actor*);
    virtual void buildDescriptionName(ItemStackBase const&);
    virtual void buildDescriptionId(ItemDescriptor const&, CompoundTag const*);
    virtual void buildEffectDescriptionName(ItemStackBase const&);
    virtual void readUserData(ItemStackBase&, IDataInput&, class ReadOnlyBinaryStream&);
    virtual void writeUserData(ItemStackBase const&, IDataOutput&);
    virtual void getMaxStackSize(ItemDescriptor const&);
    virtual void inventoryTick(ItemStack&, Level&, Actor&, int, bool);
    virtual void refreshedInContainer(ItemStackBase const&, Level&);
    virtual void getCooldownType(void);
    virtual void getCooldownTime(void);
    virtual void fixupCommon(ItemStackBase&);
    virtual void fixupCommon(ItemStackBase&, Level&);
    virtual void getInHandUpdateType(Actor const&, class ItemInstance const&, ItemInstance const&, bool, bool);
    virtual void validFishInteraction(int);
    virtual void enchantProjectile(ItemStackBase const&, Actor&);
    virtual void getEquipLocation(void);
    virtual void getEquipSound(void);
    virtual void shouldSendInteractionGameEvents(void);
    virtual void useInterruptedByAttacking(void);
    virtual bool hasSameRelevantUserData(ItemStackBase const&, ItemStackBase const&);
    virtual void initClient(/*Json::Value&, SemVersion const&, bool, Experiments const&*/);
    virtual void setIconInfo(std::string const&, int);
    virtual void getIconInfo(ItemStackBase const&, int, bool);
    virtual void getInteractText(Actor const&);
    virtual void getAnimationFrameFor(Actor*, bool, ItemStack const*, bool);
    virtual bool isEmissive(int);
    virtual void getLightEmission(int);
    virtual void getIconYOffset(void);
    virtual bool canBeCharged(void);
    virtual void playSoundIncrementally(ItemStack const&, Actor&);
    virtual void getFurnaceXPmultiplier(class ItemStackBase const&);
    virtual void getAuxValuesDescription(void);
    virtual void calculatePlacePos(ItemStackBase&, Actor&, unsigned char&, BlockPos&);
    virtual void _checkUseOnPermissions(Actor&, ItemStackBase&, unsigned char const&, BlockPos const&);
    virtual void _calculatePlacePos(ItemStackBase&, Actor&, unsigned char&, BlockPos&);
    virtual void _shouldAutoCalculatePlacePos(void);
    virtual void _useOn(ItemStack&, Actor&, BlockPos, unsigned char, Vector3 const&);
public:
    std::string GetName();
    bool IsSword();
    bool IsArmor2();
    int GetArmorSlot();
    bool IsHelmet();
    bool IsChestplate();
    bool IsLeggings();
    bool IsBoots();
    bool IsPickaxe();
    bool IsAxe();
    bool IsShovel();
    bool IsBoombox();
    bool IsBowOrArrow();
    bool IsBow();
    bool IsArrow();

    bool IsArmor();

    bool IsTool();
};


class ItemRegistry
{
public:
    static Item*** lookUpByName(std::string text) {

        using lookupByName = Item * **(__fastcall*)(void*, TextHolder&);
        static lookupByName lookupByNameF;
        if (lookupByNameF == 0x0)
        {
            uintptr_t reg = Memory::SearchAOB("E8 ? ? ? ? 90 48 8B 5C 24 ? 48 85 DB 74 ? 48 8B 13");
            lookupByNameF = reinterpret_cast<lookupByName>(reg + *reinterpret_cast<int*>(reg + 1) + 5);
        }

        std::unique_ptr<void*> void1 = std::make_unique<void*>();
        std::unique_ptr<void*> void2 = std::make_unique<void*>();
        auto temp = TextHolder(text);
        return lookupByNameF(void1.get(), temp);
    }
};



class Inventory
{
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    virtual ~Inventory();
    virtual void init(void);
    virtual void addContentChangeListener(ContainerContentChangeListener*);
    virtual void removeContentChangeListener(ContainerContentChangeListener*);
    virtual void addRemovedListener(class ContainerRemovedListener*);
    virtual void removeRemovedListener(ContainerRemovedListener*);
    virtual void Function5();
    virtual ItemStack* getItem(int);
    virtual bool hasRoomForItem(ItemStack const&);
    virtual void addItem(ItemStack&);
    virtual void addItemWithForceBalance(ItemStack&);
    virtual void addItemToFirstEmptySlot(ItemStack const&);
    virtual void setItem(int, ItemStack const&);
    virtual void setItemWithForceBalance(int, ItemStack const&, bool);
    virtual void removeItem(int, int);
    virtual void removeAllItems(void);
    virtual void removeAllItemsWithForceBalance(void);
    virtual void containerRemoved(void);
    virtual void dropSlotContent(class BlockSource&, struct Vec3 const&, bool, int);
    virtual void dropContents(class BlockSource&, struct Vec3 const&, bool);
    virtual int getContainerSize(void);
    virtual void getMaxStackSize(void);
    virtual void startOpen(Actor&);
    virtual void stopOpen(Actor&);
    virtual void getSlotCopies(void);
    virtual void getSlots(void);
    virtual void getEmptySlotsCount(void);
    virtual void getItemCount(ItemStack const&);
    virtual void findFirstSlotForItem(ItemStack const&);
    virtual bool canPushInItem(int, int, ItemStack const&);
    virtual bool canPullOutItem(int, int, ItemStack const&);
    virtual void setContainerChanged(int);
    virtual void setContainerMoved(void);
    virtual void setCustomName(std::string const&);
    virtual bool hasCustomName(void);
    virtual void readAdditionalSaveData(CompoundTag const&);
    virtual void addAdditionalSaveData(CompoundTag&);
    virtual void initializeContainerContents(BlockSource&);
    virtual bool isEmpty(void);
    virtual bool isSlotDisabled(int);
    virtual void add(ItemStack&);
    virtual bool canAdd(ItemStack const&);
    virtual void clearSlot(int);
    virtual void clearInventory(int);
    virtual void load(class ListTag const&, class SemVersion const&, class Level&);
    virtual int getFirstEmptySlot(void);
    virtual void setContainerSize(int);

    void dropSlot(int slot);
    void swapSlots(int from, int to);
    void equipArmor(int slot);
    void startUsingItem(int slot);
    void releaseUsingItem(int slot);
};

class PlayerInventoryProxy {
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0)
    BUILD_ACCESS(this, int, selectedHotbarSlot, 0x0010);
    BUILD_ACCESS(this, Inventory*, inventory, 0x00C0);

    ItemStack* getFirstItemSlot() {
        std::vector<ItemStack*> items = GetInventoryItems();
        if (!items.empty()) return items[0];
        return nullptr;
    }

    std::vector<int> GetSwords(bool hotbarOnly = true)
    {
        std::vector<int> slots;
        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("_sword"))
            {
                slots.push_back(i);
            }
        }

        return slots;
    }

    std::vector<ItemStack*> GetInventoryItems()
    {
        std::vector<ItemStack*> items;
        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            items.push_back(item);
        }
        return items;
    }


    ItemStack* GetSelectedItem()
    {
        return inventory->getItem(selectedHotbarSlot);
    }

    ItemStack* GetItemAtSlot(int slot)
    {
        return inventory->getItem(slot);
    }

    enum class Tiers
    {
        WOOD,
        STONE,
        IRON,
        DIAMOND,
        NETHERITE
        // no gold :trollblack:
    };

    int GetBestPickaxe(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;
        int best = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("pickaxe"))
            {
                int tier = 0;
                if (itemPtr->GetName().find("wood") != std::string::npos)
                    tier = 0;
                else if (itemPtr->GetName().find("stone") != std::string::npos)
                    tier = 1;
                else if (itemPtr->GetName().find("iron") != std::string::npos)
                    tier = 2;
                else if (itemPtr->GetName().find("diamond") != std::string::npos)
                    tier = 3;
                else if (itemPtr->GetName().find("netherite") != std::string::npos)
                    tier = 4;

                tier += item->getEnchantValue(Enchant::EFFICIENCY);

                if (tier > best)
                {
                    best = tier;
                    slot = i;
                }
            }
        }

        return slot;
    }

    int GetBestSword(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;
        int best = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("sword"))
            {
                int tier = 0;
                if (itemPtr->GetName().find("wood") != std::string::npos)
                    tier = 0;
                else if (itemPtr->GetName().find("stone") != std::string::npos)
                    tier = 1;
                else if (itemPtr->GetName().find("iron") != std::string::npos)
                    tier = 2;
                else if (itemPtr->GetName().find("diamond") != std::string::npos)
                    tier = 3;
                else if (itemPtr->GetName().find("netherite") != std::string::npos)
                    tier = 4;

                tier += item->getEnchantValue(Enchant::SHARPNESS);

                if (tier > best)
                {
                    best = tier;
                    slot = i;
                }
            }
        }

        return slot;
    }

    int GetGoldSword(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("golden_sword"))
            {
                slot = i;
                break;
            }
        }

        return slot;
    }

    int GetBestAxe(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;
        int best = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("_axe"))
            {
                int tier = 0;
                if (itemPtr->GetName().find("wood") != std::string::npos)
                    tier = 0;
                else if (itemPtr->GetName().find("stone") != std::string::npos)
                    tier = 1;
                else if (itemPtr->GetName().find("iron") != std::string::npos)
                    tier = 2;
                else if (itemPtr->GetName().find("diamond") != std::string::npos)
                    tier = 3;
                else if (itemPtr->GetName().find("netherite") != std::string::npos)
                    tier = 4;

                tier += item->getEnchantValue(Enchant::EFFICIENCY);

                if (tier > best)
                {
                    best = tier;
                    slot = i;
                }
            }
        }

        return slot;
    }

    int GetBestShovel(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;
        int best = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (!item->item)
                continue;
            if (hotbarOnly && i > 8)
                continue;
            if (itemPtr->GetName().contains("shovel"))
            {
                int tier = 0;
                if (itemPtr->GetName().find("wood") != std::string::npos)
                    tier = 0;
                else if (itemPtr->GetName().find("stone") != std::string::npos)
                    tier = 1;
                else if (itemPtr->GetName().find("iron") != std::string::npos)
                    tier = 2;
                else if (itemPtr->GetName().find("diamond") != std::string::npos)
                    tier = 3;
                else if (itemPtr->GetName().find("netherite") != std::string::npos)
                    tier = 4;

                tier += item->getEnchantValue(Enchant::EFFICIENCY);

                if (tier > best)
                {
                    best = tier;
                    slot = i;
                }
            }
        }

        return slot;
    }

    int GetToolItemValue(ItemStack* itemStack)
    {

        if (!this) return 0; // ???
        int tier = 0;
        if (!itemStack || !itemStack->item)
            return 0;

        Item* item = *itemStack->item;

        // if its a sword, add sharpness

        if (item->GetName().find("wood") != std::string::npos)
            tier = 0;
        else if (item->GetName().find("stone") != std::string::npos)
            tier = 1;
        else if (item->GetName().find("iron") != std::string::npos)
            tier = 2;
        else if (item->GetName().find("diamond") != std::string::npos)
            tier = 3;
        else if (item->GetName().find("netherite") != std::string::npos)
            tier = 4;

        if (item->GetName().find("sword") != std::string::npos)
            tier += itemStack->getEnchantValue(Enchant::SHARPNESS);
        else
            tier += itemStack->getEnchantValue(Enchant::EFFICIENCY);


        return tier;
    }



    int GetPlaceableBlock(bool hotbarOnly = true)
    {
        if (!this) return 0; // ???
        int slot = -1;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (hotbarOnly && i > 8)
                continue;
            if (item->blockPtr)
            {
                if (itemPtr->GetName().find("netherreactor") != std::string::npos) continue;
                if (itemPtr->GetName().find("boombox") != std::string::npos) continue;
                if (itemPtr->GetName().find("lilypad") != std::string::npos) continue;


                slot = i;
                break;
            }
        }

        return slot;
    }

    int GetAllPlaceableBlocks(bool hotbarOnly = true) {
        if (!this) return 0; // ???
        int amount = 0;

        for (int i = 0; i < 36; i++)
        {
            ItemStack* item = inventory->getItem(i);
            if (!item)
                continue;
            if (!item->item)
                continue;
            Item* itemPtr = *item->item;
            if (hotbarOnly && i > 8)
                continue;
            if (item->blockPtr)
            {
                if (itemPtr->GetName().find("netherreactor") != std::string::npos) continue;
                if (itemPtr->GetName().find("boombox") != std::string::npos) continue;
                if (itemPtr->GetName().find("lilypad") != std::string::npos) continue;

                amount += item->amount;
            }
        }

        return amount;
    }

    bool IsToolItem(Item *pItem) {
        if (pItem->GetName().find("_sword") != std::string::npos)
            return true;
        if (pItem->GetName().find("_axe") != std::string::npos)
            return true;
        if (pItem->GetName().find("_shovel") != std::string::npos)
            return true;
        if (pItem->GetName().find("_pickaxe") != std::string::npos)
            return true;
        return false;
    }
};