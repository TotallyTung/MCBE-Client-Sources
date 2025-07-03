#include "MidClick.h"
#include "../../FriendList/FriendsManager.h"

bool ep = false;
MidClick::MidClick() : IModule(0, Category::PLAYER, "Click a player with your mouse wheel to add em as a friend.") {
	registerBoolSetting("EnderPearl", &ep, false);
	registerBoolSetting("tell", &tell, true);
}

MidClick::~MidClick() {
}

const char* MidClick::getModuleName() {
	return "MidClick";
}

void MidClick::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	Entity* entity = Game.getLocalPlayer()->level->getEntity();
	if (entity == nullptr) {
		if (ep && GameData::isWheelDown()) {
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			Inventory* inv = supplies->inventory;
			int slotab = supplies->selectedHotbarSlot;
			for (int n = 0; n < 36; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->isValid()) {
					if (stack->getItem()->itemId == 422) {
						Game.getLocalPlayer()->switchHotbar(n);
						gm->baseUseItem(*stack);
						Game.getLocalPlayer()->switchHotbar(slotab);
					};
				};
			};
		};
		return;
	};
	if (entity->getEntityTypeId() != 319) return;
	std::string name = entity->getNameTag()->getText();

	if (GameData::isWheelDown() && !hasClicked) {
		if (!FriendsManager::findFriend(name)) {
			FriendsManager::addFriendToList(name);
			Game.getGuiData()->displayClientMessageF("%sSuccessfully added %s %sto your friendlist.", GREEN, name.c_str(), GREEN);
			if (name.find(u8" ") != std::string::npos) name = '"' + name + '"';
			if (tell) Utils::sendCommand("tell " + name + u8" ‚ ‚È‚½‚ğFriendList‚É’Ç‰Á‚µ‚Ü‚µ‚½ (Added you to FriendList) By MidClick");
		}
		else {
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			FriendsManager::removeFriend(name);
			Game.getGuiData()->displayClientMessageF("%sSuccessfully removed %s %sfrom your friendlist.", GREEN, name.c_str(), GREEN);
			if (name.find(u8" ") != std::string::npos) name = '"' + name + '"';
			if (tell) Utils::sendCommand("tell " + name + u8" ‚ ‚È‚½‚ğFriendList‚©‚çíœ‚µ‚Ü‚µ‚½ (removed you from FriendList) By MidClick");
		}
		hasClicked = true;
	}
	else if (!GameData::isWheelDown()) hasClicked = false;
};
