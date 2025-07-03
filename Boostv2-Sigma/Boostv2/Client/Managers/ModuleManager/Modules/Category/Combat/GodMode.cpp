#include "GodMode.h"

GodMode::GodMode() : Module("GodMode", "Made by Zeppi iziziz", Category::COMBAT) {
}
void GodMode::onNormalTick(LocalPlayer* localPlayer) {
	Container* inventory = localPlayer->playerInventory->container;
	int equipped = 0;

	for (int i = 0; i < 36; i++) {
		ItemStack* itemStack = inventory->getItem(i);
		Item* item = itemStack->item.get();

		if (item != nullptr && item->itemId == 355) {
			if (equipped == 0 && (localPlayer->getArmor(ArmorSlot::Helmet)->item.get() == nullptr || localPlayer->getArmor(ArmorSlot::Helmet)->item->itemId != 355)) {
				InventoryAction first(i, itemStack, ItemStack::getEmptyItem());
				InventoryAction second(0, ItemStack::getEmptyItem(), itemStack, ContainerID::Armor);
				std::unique_ptr<ComplexInventoryTransaction> cit = std::make_unique<ComplexInventoryTransaction>();
				cit->type = ComplexInventoryTransaction::Type::NormalTransaction;
				cit->data.addAction(first);
				cit->data.addAction(second);
				InventoryTransactionPacket pk(std::move(cit));
				localPlayer->level->getPacketSender()->sendToServer(&pk);
				equipped++;
				continue;
			}
			if (equipped <= 1 && (localPlayer->getArmor(ArmorSlot::Chestplate)->item.get() == nullptr || localPlayer->getArmor(ArmorSlot::Chestplate)->item->itemId != 355)) {
				InventoryAction first(i, itemStack, ItemStack::getEmptyItem());
				InventoryAction second(1, ItemStack::getEmptyItem(), itemStack, ContainerID::Armor);
				std::unique_ptr<ComplexInventoryTransaction> cit = std::make_unique<ComplexInventoryTransaction>();
				cit->type = ComplexInventoryTransaction::Type::NormalTransaction;
				cit->data.addAction(first);
				cit->data.addAction(second);
				InventoryTransactionPacket pk(std::move(cit));
				localPlayer->level->getPacketSender()->sendToServer(&pk);
				equipped++;
				continue;
			}
			if (equipped <= 2 && (localPlayer->getArmor(ArmorSlot::Leggings)->item.get() == nullptr || localPlayer->getArmor(ArmorSlot::Leggings)->item->itemId != 355)) {
				InventoryAction first(i, itemStack, ItemStack::getEmptyItem());
				InventoryAction second(2, ItemStack::getEmptyItem(), itemStack, ContainerID::Armor);
				std::unique_ptr<ComplexInventoryTransaction> cit = std::make_unique<ComplexInventoryTransaction>();
				cit->type = ComplexInventoryTransaction::Type::NormalTransaction;
				cit->data.addAction(first);
				cit->data.addAction(second);
				InventoryTransactionPacket pk(std::move(cit));
				localPlayer->level->getPacketSender()->sendToServer(&pk);
				equipped++;
				continue;
			}
			if (equipped <= 3 && (localPlayer->getArmor(ArmorSlot::Boots)->item.get() == nullptr || localPlayer->getArmor(ArmorSlot::Boots)->item->itemId != 355)) {
				InventoryAction first(i, itemStack, ItemStack::getEmptyItem());
				InventoryAction second(3, ItemStack::getEmptyItem(), itemStack, ContainerID::Armor);
				std::unique_ptr<ComplexInventoryTransaction> cit = std::make_unique<ComplexInventoryTransaction>();
				cit->type = ComplexInventoryTransaction::Type::NormalTransaction;
				cit->data.addAction(first);
				cit->data.addAction(second);
				InventoryTransactionPacket pk(std::move(cit));
				localPlayer->level->getPacketSender()->sendToServer(&pk);
				equipped++;
				continue;
			}
		}
	}
}