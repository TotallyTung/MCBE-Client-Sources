#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "<count> <mode: give / offhand : 1/0>") {
	registerAlias("d");
}

DupeCommand::~DupeCommand() {
}

bool DupeCommand::execute(std::vector<std::string>* args) {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	auto transactionManager = Game.getLocalPlayer()->getTransactionManager();
	Inventory* inv = supplies->inventory;
	int selectedSlot = supplies->selectedHotbarSlot;
	ItemStack* item = inv->getItemStack(selectedSlot);
	int count = item->count;
	bool isGive = true;
	if (args->size() > 1)
		item->count = assertInt(args->at(1));
	if (args->size() > 2)
		isGive = static_cast<bool>(assertInt(args->at(2)));
	if (isGive) {
		InventoryAction* firstAction = nullptr;
		InventoryAction* secondAction = nullptr;
		firstAction = new InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*firstAction);
		inv->addItemToFirstEmptySlot(item);
	} else {
		InventoryAction* pp = nullptr;
		pp = new InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*pp);
		Game.getLocalPlayer()->setOffhandSlot(item);
	}
	if (args->size() > 1)
		item->count = count;
		clientMessageF(u8"%s%sSuccessfully duplicated the item!", Utils::getCommandClientName(), GREEN);
	return true;
}
