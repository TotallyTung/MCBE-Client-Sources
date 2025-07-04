/*#include "AutoArmor.h"

AutoArmor::AutoArmor() : Module("AutoArmor", "Modify your swings speed.", Category::PLAYER) {
	addSlider<int>("Speed", "NULL", ValueType::INT_T, &swingSpeed, 1, 20);
}
class ArmorStruct {
public:
	ArmorStruct(ItemStack* item, ArmorItem* yot, int slot) {
		armor = yot;
		m_slot = slot;
		m_item = item;
	}
	bool isEqual(ArmorStruct& src) {
		if (m_item->getItemPtr()->getArmorValue() == src.m_item->getItemPtr()->getArmorValue())
			return true;
		else
			return false;
	}

	bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
		return (lhs.m_item->getItemPtr()->getArmorValue() > rhs.m_item->getItemPtr()->getArmorValue());
	}
	ArmorItem* armor = nullptr;
	ItemStack* m_item = nullptr;
	int m_slot = 0;
};
void AutoArmor::onNormalTick(Actor* actor) {
	
	GameMode* gm = mc.getGameMode();
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	InventoryTransactionManager* manager = mc.getLocalPlayer()->getInventoryTransactionManager();

	InventoryAction* first = nullptr;
	InventoryAction* second = nullptr;
	static ItemStack* emptyItemStack = nullptr;

	if (emptyItemStack == 0x0) {
		uintptr_t sigOffset = findSig(Sigs::Itemstack::Emptyitemstack);
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emptyItemStack = reinterpret_cast<ItemStack*>(sigOffset + offset + 7 /*length of instruction*///);
	/*}

	std::vector<ArmorStruct> armorList;

	struct CompareArmorStruct {
		bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
			return (lhs.m_item->getItemPtr()->getArmorValue() > rhs.m_item->getItemPtr()->getArmorValue());
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 36; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<ArmorItem*>(*stack->item)->ArmorSlot == i) {
				armorList.push_back(ArmorStruct(stack, reinterpret_cast<ArmorItem*>(*stack->item), n));
			}
		}

		if (gm->player->getArmor(i)->item != nullptr)
			armorList.push_back(ArmorStruct(gm->player->getArmor(i), reinterpret_cast<ArmorItem*>(*gm->player->getArmor(i)->item), i));

		if (armorList.size() > 0) {
			std::sort(armorList.begin(), armorList.end(), CompareArmorStruct());
			ItemStack* armorItem = gm->player->getArmor(i);

			if (armorItem->item == nullptr) {
				*mc.getLocalPlayer()->getArmor(i) = *inv->getItemStack(armorList[0].m_slot);

				first = new InventoryAction(
					InventorySource{ InventorySourceType::ContainerInventory, 120, InventorySourceFlags::NoFlag },
					i,
					ItemStack(*armorList[0].m_item), // Convert to NetworkItemStackDescriptor if necessary
					ItemStack(*armorList[0].m_item)  // Convert to NetworkItemStackDescriptor if necessary
				);
				second = new InventoryAction(slot, nullptr, armorItem);

				*mc.getLocalPlayer()->getArmor(i) = *emptyItemStack;
				*inv->getItemStack(slot) = *armorItem;

				manager->addAction(first);
				manager->addAction(second);

				delete first;
				delete second;

				first = new InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
				second = new InventoryAction(i, nullptr, armorList[0].m_item, InventorySource(InventorySourceType::ContainerInventory, ContainerID::Armor, InventorySourceFlags::NoFlag));

				*mc.getLocalPlayer()->getArmor(i) = *inv->getItemStack(armorList[0].m_slot);
				*inv->getItemStack(armorList[0].m_slot) = *emptyItemStack;

				manager->addAction(first);
				manager->addAction(second);

				delete first;
				delete second;
			}
			if (armorItem->item == nullptr) {
				*mc.getLocalPlayer()->getArmor(i) = *inv->getItemStack(armorList[0].m_slot);

				first = new InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
				second = new InventoryAction(i, nullptr, armorList[0].m_item, InventorySource(ContainerInventory, Armor, NoFlag));

				*inv->getItemStack(armorList[0].m_slot) = *emptyItemStack;

				manager->addAction(first);
				manager->addAction(second);

				delete first;
				delete second;
			}
		}
		armorList.clear();
	}
	armorList.clear();
}*/