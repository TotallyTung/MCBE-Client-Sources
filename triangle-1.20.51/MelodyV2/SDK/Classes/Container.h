#pragma once
#include <functional>
#include "ItemStack.h"
#include "../../Utils/Math.h"

#include <functional>
#include <string>
#include <stdint.h>
#include <vector>

enum class ContainerType : uint8_t
{
	CHEST = 0,
	CRAFTING_TABLE = 1,
	FURNACE = 2,
	ENCHANTMENT_TABLE = 3,
	ANVIL = 5,
	DISPENSER = 6,
	DROPPER = 7,
	HOPPER = 8,
	BEACON = 13,
	GRINDSTONE = 26,
	BLAST_FURNACE = 27,
	SMOKER = 28,
	STONECUTTER = 29,
	SMITHING_TABLE = 33,
	INVENTORY = 0xFF
};

struct ContainerRuntimeIdTag {};

class Container {
public:
	virtual ~Container();
	virtual void init();
	virtual void serverInitItemStackIds(int param_1, int param_2, std::function<void __cdecl(int, const ItemStack&)>* param_3);
	virtual void addContentChangeListener(class ContainerContentChangeListener* param_1);
	virtual void removeContentChangeListener(class ContainerContentChangeListener* param_1);
	virtual ItemStack* getItemStack(int param_1);
	virtual bool hasRoomForItem(ItemStack* param_1);
	virtual bool addItem(ItemStack* param_1);
	virtual bool addItemWithForceBalance(ItemStack* param_1);
	virtual bool addItemToFirstEmptySlot(ItemStack* param_1);
	virtual void setItem(int param_1, ItemStack* param_2);
	virtual void setItemWithForceBalance(int param_1, ItemStack* param_2, bool param_3);
	virtual void removeItem(int param_1, int param_2);
	virtual void removeAllItems();
	virtual void removeAllItemsWithForceBalance();
	virtual void dropContents(class BlockSource* param_1, class Vec3<float>* param_2, bool param_3);
	virtual int getContainerSize();
	virtual int getMaxStackSize();
	virtual void UndefinedFunc_18(void* param_2, void* param_3);
	virtual void UndefinedFunc_19(void* param_2, void* param_3);
	virtual std::vector<ItemStack>* getSlotCopies();
	virtual const std::vector<const ItemStack*>& getSlots();
	virtual int getEmptySlotsCount();
	virtual int getItemCount(ItemStack* param_1);
	virtual int findFirstSlotForItem(ItemStack* param_1);
	virtual bool hasNetworkSession();
	virtual bool hasNetworkSession_1();
	virtual void setContainerChanged(int param_1);
	virtual void setContainerMoved();
	virtual void setCustomName(const std::string& param_1);
	virtual bool getIsWaxed();
	virtual void readAdditionalSaveData(class CompoundTag* param_1);
	virtual void addAdditionalSaveData(class CompoundTag* param_1);
	virtual void createTransactionContext(std::function<void __cdecl(Container&, int, const ItemStack&, const ItemStack&)>* param_1, std::function<void __cdecl(void)> param_2);
	virtual void UndefinedFunc_34(void* param_2, void* param_3);
	virtual bool isEmpty();
	virtual bool add(class ItemStack* param_1);
	virtual bool canAdd(class ItemStack* param_1);
	virtual void clearSlot(int param_1);
	virtual int clearInventory(int param_1);
	virtual void load(class ListTag* param_1, class SemVersion* param_2, class Level* param_3);
	virtual int getFirstEmptySlot();
	virtual void setContainerSize(int param_1);
};