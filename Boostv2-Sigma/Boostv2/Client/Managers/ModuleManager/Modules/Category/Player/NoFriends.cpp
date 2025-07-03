#include "NoFriends.h"
#include "MidClick.h"
#include "../../../../../Client.h"

NoFriends::NoFriends() : Module("NoFriends", "Temporarily remove all friends from the friends list.", Category::PLAYER) {}

void NoFriends::onEnable() {
	MidClick* midClick = ModuleManager::getModule<MidClick>(); // ADD MidClick
	originalFriends = midClick->getFriends();
	midClick->getFriends().clear();
	Client::DisplayClientMessage("%sAll friends have been temporarily removed.", MCTF::GREEN);
}

void NoFriends::onDisable() {
	MidClick* midClick = ModuleManager::getModule<MidClick>(); // ADD MidClick
	midClick->getFriends() = originalFriends;
	Client::DisplayClientMessage("%sAll friends have been restored.", MCTF::GREEN);
}