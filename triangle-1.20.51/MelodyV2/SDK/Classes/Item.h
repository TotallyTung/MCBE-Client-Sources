#pragma once
#include "../../Utils/ColorUtils.h"
#include "../../Utils/Math.h"
#include "HashedString.h"
#include "WeakPtr.h"
#include <string>

#include "../Signatures.h"

class Actor;
class Mob;
class Player;
class Level;
class Container;
class CompoundTag;
class SemVersion;
class Experiments;
enum RenderParams;
class BaseGameVersion;
class ItemDescriptor;
class ItemInstance;
class ItemStackBase;
class ItemStack;
class IDataInput;
class IDataOutput;
class ReadOnlyBinaryStream;

class Block;
class BlockSource;

class Item {
public:
	//std::string texture_name; //0x0060
	BUILD_ACCESS(this, int, itemId, 0xA2);
	BUILD_ACCESS(this, std::string, texture_name, 0x60);
	BUILD_ACCESS(this, std::vector<HashedString>, recipetags, 0x240);
	//BUILD_ACCESS(this, WeakPtr<BlockLegacy>, blockLegacy, 0x1C8);
	//48 89 5c 24 ? 57 48 83 ec ? 48 8b da 48 85 d2
	__int64 getEnchantLevel(CompoundTag* a2) {
		using func_t = __int64(__thiscall*)(Item*,CompoundTag*);
		static func_t func = (func_t)findSig(Sigs::Itemstack::getmaxdamagevalue);
		return func(this, a2);
	}
	Item* setAllowOffhand(bool allow = true) {
		using func_t = Item * (__thiscall*)(Item*, bool);
		static func_t func = (func_t)findSig(Sigs::Itemstack::setallowoofhand);
		return func(this, allow);
	}
public:

	virtual ~Item();
	virtual void initServer(__int64, SemVersion const&, bool, Experiments const&);
	virtual void tearDown(void);
	virtual void setDescriptionId(std::string const&);
	virtual void getDescriptionId(void);
	virtual int getMaxUseDuration(ItemStack const*);
	virtual bool isMusicDisk(void);
	virtual void executeEvent(ItemStackBase&, std::string const&, RenderParams&);
	virtual bool isComponentBased(void);
	virtual bool isArmor(void);
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
	virtual void getComponent(HashedString const&);
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
	virtual int getAttackDamage(void);
	virtual bool isHandEquipped(void);
	virtual bool isGlint(ItemStackBase const&);
	virtual bool isPattern(void);
	virtual void getPatternIndex(void);
	virtual void showsDurabilityInCreative(void);
	virtual bool isWearableThroughLootTable(CompoundTag const*);
	virtual bool canDestroyInCreative(void);
	virtual bool isDestructive(int);
	virtual bool isLiquidClipItem(void);
	virtual void shouldInteractionWithBlockBypassLiquid(Block const&);
	virtual void requiresInteract(void);
	virtual void appendFormattedHovertext(ItemStackBase const&, Level&, std::string&, bool);
	virtual bool isValidRepairItem(ItemStackBase const&, ItemStackBase const&, BaseGameVersion const&);
	virtual void getEnchantSlot(void);
	virtual void getEnchantValue(void);
	virtual __int64 getArmorValue(void);
	virtual void getToughnessValue(void);
	virtual bool isComplex(void);
	virtual bool isValidAuxValue(int);
	virtual void getDamageChance(int);
	virtual void getViewDamping(void);
	virtual void uniqueAuxValues(void);
	virtual bool isActorPlacerItem(void);
	virtual bool isMultiColorTinted(ItemStack const&);
	virtual void getColor(CompoundTag const*, ItemDescriptor const&);
	virtual bool hasCustomColor(ItemStackBase const&);
	virtual bool hasCustomColor(CompoundTag const*);
	virtual void clearColor(ItemStackBase&);
	virtual void clearColor(CompoundTag*);
	virtual void setColor(ItemStackBase*, MC_Color const&);
	virtual void getBaseColor(ItemStack const&);
	virtual void getSecondaryColor(ItemStack const&);
	virtual void getActorIdentifier(ItemStack const&);
	virtual void buildIdAux(short, CompoundTag const*);
	virtual bool canUseOnSimTick(void);
	virtual void use(ItemStack&, Player&);
	virtual void dispense(BlockSource&, Container&, int, Vec3<float> const&, uint8_t);
	virtual void useTimeDepleted(ItemStack&, Level*, Player*);
	virtual void releaseUsing(ItemStack&, Player*, int);
	virtual float getDestroySpeed(ItemStackBase const&, Block const&);
	virtual void hurtActor(ItemStack&, Actor&, Mob&);
	virtual void hitActor(ItemStack&, Actor&, Mob&);
	virtual void hitBlock(ItemStack&, Block const&, Vec3<int> const&, Mob&);
	virtual void mineBlock(ItemInstance&, Block const&, int, int, int, Actor*);
	virtual void mineBlock(ItemStack&, Block const&, int, int, int, Actor*);
	virtual void buildDescriptionName(ItemStackBase const&);
	virtual void buildDescriptionId(ItemDescriptor const&, CompoundTag const*);
	virtual void buildEffectDescriptionName(ItemStackBase const&);
	virtual void readUserData(ItemStackBase&, IDataInput&, ReadOnlyBinaryStream&);
	virtual void writeUserData(ItemStackBase const&, IDataOutput&);
	virtual __int64 getMaxStackSize(ItemDescriptor const*);
	virtual void inventoryTick(ItemStack&, Level&, Actor&, int, bool);
	virtual void refreshedInContainer(ItemStackBase const&, Level&);
	virtual void getCooldownType(void);
	virtual void getCooldownTime(void);
	virtual void fixupCommon(ItemStackBase&);
	virtual void fixupCommon(ItemStackBase&, Level&);
	virtual void getInHandUpdateType(Player const&, ItemInstance const&, ItemInstance const&, bool, bool);
	virtual void getInHandUpdateType(Player const&, ItemStack const&, ItemStack const&, bool, bool);
	virtual void validFishInteraction(int);
	virtual void enchantProjectile(ItemStackBase const&, Actor&);
	virtual void getEquipLocation(void);
	virtual void getEquipSound(void);
	virtual void shouldSendInteractionGameEvents(void);
	virtual void useInterruptedByAttacking(void);
	virtual bool hasSameRelevantUserData(ItemStackBase const&, ItemStackBase const&);
	virtual void initClient(__int64, SemVersion const&, bool, Experiments const&);
	virtual void setIconInfo(std::string const&, int);
	virtual void getIconInfo(ItemStackBase const&, int, bool);
	virtual void getInteractText(Player const&);
	virtual void getAnimationFrameFor(Mob*, bool, ItemStack const*, bool);
	virtual bool isEmissive(int);
	virtual void getLightEmission(int);
	virtual void getIconYOffset(void);
	virtual bool canBeCharged(void);
	virtual void playSoundIncrementally(ItemStack const&, Mob&);
	virtual void getFurnaceXPmultiplier(ItemStackBase const&);
	virtual void getAuxValuesDescription(void);
	virtual void calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vec3<int>&);
	virtual void _checkUseOnPermissions(Actor&, ItemStackBase&, uint8_t const&, Vec3<int> const&);
	virtual void _calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vec3<int>&);
	virtual void _shouldAutoCalculatePlacePos(void);
	virtual void _useOn(ItemStack*, Actor*, Vec3<int>, uint8_t, Vec3<float> const&);
	bool hasRecipeTag(const std::string& tag) const
    {
        for (const std::string& rtag : this->recipetags)
        {
            if (rtag == tag)
            {
                return true;
            }
        }
        return false;
    }
};
/*enum class ArmorSlot : int32_t
{
	HEAD = 0,
	CHESTPLATE = 1,
	LEGGINGS = 2,
	BOOTS = 3
};*/

class ArmorItem : public Item
{
public:
	//ArmorSlot armorSlot; //0x0258
	
	int32_t armorValue; //0x025C
	char pad_0260[16]; //0x0260
	int ArmorSlot;  //0x0258

	bool isHelmet() {
		return ArmorSlot == 0;
	}
	bool isChestplate() {
		return ArmorSlot == 1;
	}
	bool isLeggings() {
		return ArmorSlot == 2;
	}
	bool isBoots() {
		return ArmorSlot == 3;
	}
	/*
		int getArmorValue() const
		{
			return this->armorValue;
			//return vftCall<64, float, const Item*>(this);
		}
		float getArmorKnockbackResistance() const
		{
			return vftCall<136, float, const ArmorItem*>(this);
		}*/
};