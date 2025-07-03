#pragma once

class Actor;
class Mob;
class Player;
class Level;
class Container;
class CompoundTag;
class Experiments;
struct RenderParams;
class BaseGameVersion;
class ItemDescriptor;
class ItemStackBase;
class ItemInstance {
public:
	// vIndex: 0
	virtual ~ItemInstance();

	// vIndex: 1
	virtual void reinit(class Item const& item, int count, int auxValue);

	// vIndex: 2
	virtual void reinit(class BlockLegacy const& block, int count);

	// vIndex: 3
	virtual void reinit(std::string_view name, int count, int auxValue);
};
class ItemStack;
class IDataInput;
class IDataOutput;
class ReadOnlyBinaryStream;

enum class EnchantType : int8_t {
	Protection = 0x0,
	FireProtection = 0x1,
	FeatherFalling = 0x2,
	BlastProtection = 0x3,
	ProjectileProtection = 0x4,
	Thorns = 0x5,
	Respiration = 0x6,
	DepthStrider = 0x7,
	AquaAffinity = 0x8,
	Sharpness = 0x9,
	Smite = 0xA,
	BaneOfArthropods = 0xB,
	Knockback = 0xC,
	FireAspect = 0xD,
	Looting = 0xE,
	Efficiency = 0xF,
	SilkTouch = 0x10,
	Unbreaking = 0x11,
	Fortune = 0x12,
	Power = 0x13,
	Punch = 0x14,
	Flame = 0x15,
	Infinity = 0x16,
	LuckOfTheSea = 0x17,
	Lure = 0x18,
	FrostWalker = 0x19,
	Mending = 0x1A,
	CurseOfBinding = 0x1B,
	CurseOfVanishing = 0x1C,
	Impaling = 0x1D,
	Riptide = 0x1E,
	Loyalty = 0x1F,
	Channeling = 0x20,
	Multishot = 0x21,
	Piercing = 0x22,
	QuickCharge = 0x23,
	SoulSpeed = 0x24,
	SwiftSneak = 0x25,
	COUNT = 0x26,
	INVALID = 0x27,
};

enum class ItemType
{
	Helmet,
	Chestplate,
	Leggings,
	Boots,
	Sword,
	Pickaxe,
	Axe,
	Shovel,
	None
};

class Item
{
public:
	bool isAxe(void) {
		return nameContains("_axe");
	}

	bool isBoomBox() {
		return nameContains("boombox");
	}


	bool isPickaxe(void) {
		return nameContains("_pickaxe");
	}

	bool isHelmet() {
		return nameContains("_helmet");
	}

	bool isChestplate() {
		return nameContains("_chestplate");
	}

	bool isLeggings() {
		return nameContains("_leggings");
	}

	bool isBoots() {
		return nameContains("_boots");
	}

	bool isSword() {
		return nameContains("_sword");
	}
	
	bool isShovel() {
		return nameContains("_shovel");
	}

	int getArmorSlot() {
		// Use the name of the item to determine the armor slot
		static constexpr std::array<std::string_view, 4> armorSlots = {
			"_helmet",
			"_chestplate",
			"_leggings",
			"_boots"
		};

		for (int i = 0; i < armorSlots.size(); i++) {
			if (name.find(armorSlots[i]) != std::string::npos) {
				return i;
			}
		}

		return -1;
	}

	int getItemTier() {
		// Use the name of the item to determine the tier
		static constexpr std::array<std::string_view, 6> tiers = {
			"wooden_",
			"stone_",
			"iron_",
			"golden_",
			"diamond_",
			"netherite_"
		};

		for (int i = 0; i < tiers.size(); i++)
		{
			if (name.starts_with(tiers[i])) {
				return i;
			}
		}

		return 0;
	}

	ItemType getItemType() {
		if (isHelmet()) return ItemType::Helmet;
		if (isChestplate()) return ItemType::Chestplate;
		if (isLeggings()) return ItemType::Leggings;
		if (isBoots()) return ItemType::Boots;
		if (isSword()) return ItemType::Sword;
		if (isPickaxe()) return ItemType::Pickaxe;
		if (isAxe()) return ItemType::Axe;
		if (isShovel()) return ItemType::Shovel;

		return ItemType::None;
	}

	int getArmorValueFromName() {
		if (!isHelmet() && !isChestplate() && !isLeggings() && !isBoots()) return 0;
		if (nameContains("leather") || nameContains("golden")) return 1;
		if (nameContains("chainmail")) return 2;
		if (nameContains("iron")) return 3;
		if (nameContains("diamond")) return 4;
	}

	int getSwordValueFromName() {
		if (!isSword()) return 0;
		if (nameContains("wood") || nameContains("golden")) return 1;
		if (nameContains("stone")) return 2;
		if (nameContains("iron")) return 3;
		if (nameContains("diamond")) return 4;
	}

	bool isShooter(void) {
		//if ((name).find("bow") != std::string::npos) return true;
		//if ((name).find("arrow") != std::string::npos) return true;
		if (itemId == 300) return true;
		return false;
	}

	bool isThrowablePotion() {
		if ((name).find("splash_potion") != std::string::npos || (name).find("lingering_potion") != std::string::npos) return true;
		return false;
	}

	bool nameContains(const std::string& itemName) {
		if ((name).find(itemName) != std::string::npos) return true;
		return false;
	}

	CLASS_FIELD(int, itemId, 0xA2);
	CLASS_FIELD(int, mProtection, 0x264);
	CLASS_FIELD(bool, IsGlint, 0x14A); // Item::setIsGlint
	CLASS_FIELD(uint16_t, maxStackSize, 0xA0); // ComponentItem::getMaxStackSize(void)
	CLASS_FIELD(int16_t, MaxUseDuration, 0x14C);
	CLASS_FIELD(std::string, tileName, 0xC8); //this->mRawNameId in china db (find in constructor of Item class)
	CLASS_FIELD(std::string, name, 0xD8); //this->mRawNameId in china db (find in constructor of Item class)
	CLASS_FIELD(std::string, nameSpace, 0x100); // this->mRawNameId in china db (find in constructor of Item class)
public:
	virtual ~Item();
	virtual void initServer(__int64, class SemVersion const&, bool, Experiments const&);
	virtual void tearDown(void);
	virtual void setDescriptionId(std::string const&);
	virtual void getDescriptionId(void);
	virtual void getMaxUseDuration(ItemStack const*);
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
	virtual bool isTrimAllowed(void);
	virtual void getComponent();
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
	virtual __int64 getAttackDamage(void);
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
	virtual __int64 getEnchantValue(void);
	virtual __int64 getArmorValue(void);
	virtual __int64 getToughnessValue(void);
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
	virtual void setColor(ItemStackBase&, Color const&);
	virtual void getBaseColor(ItemStack const&);
	virtual void getSecondaryColor(ItemStack const&);
	virtual void getActorIdentifier(ItemStack const&);
	virtual void buildIdAux(short, CompoundTag const*);
	virtual bool canUseOnSimTick(void);
	virtual void use(ItemStack&, Player&);
	virtual void dispense(class BlockSource&, Container&, int, Vector3<float> const&, uint8_t);
	virtual void useTimeDepleted(ItemStack&, Level*, Player*);
	virtual void releaseUsing(ItemStack&, Player*, int);
	virtual  __int64 getDestroySpeed(ItemStack const&, Block const&);
	virtual void hurtActor(ItemStack&, Actor&, Mob&);
	virtual void hitActor(ItemStack&, Actor&, Mob&);
	virtual void hitBlock(ItemStack&, Block const&, Vector3<int> const&, Mob&);
	virtual void mineBlock(ItemStack&, Block const&, int, int, int, Actor*);
	virtual void buildDescriptionName(ItemStackBase const&);
	virtual void buildDescriptionId(ItemDescriptor const&, CompoundTag const*);
	virtual void buildEffectDescriptionName(ItemStackBase const&);
	virtual void readUserData(ItemStackBase&, IDataInput&, ReadOnlyBinaryStream&);
	virtual void writeUserData(ItemStackBase const&, IDataOutput&);
	virtual void getMaxStackSize(ItemDescriptor const&);
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
	virtual void initClient(__int64, class SemVersion const&, bool, Experiments const&);
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
	virtual void calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vector3<int>&);
	virtual void _checkUseOnPermissions(Actor&, ItemStackBase&, uint8_t const&, Vector3<int> const&);
	virtual void _calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vector3<int>&);
	virtual void _shouldAutoCalculatePlacePos(void);
	virtual void _useOn(ItemStack&, Actor&, Vector3<int>, uint8_t, Vector3<float> const&);
};

class ArmorItem : public Item {
public:
	int armorSlot; //0x0258
	int32_t armorValue; //0x025C
	char pad_0260[16]; //0x0260

	bool isHelmet() {
		return armorSlot == 0;
	}
	bool isChestplate() {
		return armorSlot == 1;
	}
	bool isLeggings() {
		return armorSlot == 2;
	}
	bool isBoots() {
		return armorSlot == 3;
	}
};
