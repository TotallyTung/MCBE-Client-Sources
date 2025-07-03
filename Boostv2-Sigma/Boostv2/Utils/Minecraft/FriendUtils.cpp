#include "FriendUtils.h"
#include "../../Client/Client.h"

std::vector<std::string> friendList;
void FriendUtils::addToList(Actor* actor) {
	std::string ign = *actor->getNameTag();
	friendList.push_back(ign);
}

void FriendUtils::addToList(std::string name) {
	friendList.push_back(name);
}

void FriendUtils::removeFromList(Actor* actor) {
	if (friendList.empty()) return;
	std::string ign = *actor->getNameTag();
	for (int i = 0; i < friendList.size(); i++) {
		if (ign == friendList[i]) {
			friendList.erase(friendList.begin() + i);
			break;
		}
	}
}

void FriendUtils::clearList() {
	friendList.clear();
}

void FriendUtils::loadList(std::vector<std::string> list) {
	friendList = list;
}

std::vector<std::string> FriendUtils::getList() {
	return friendList;
}

bool FriendUtils::isInList(Actor* actor) {
	std::string ign = *actor->getNameTag();
	if (friendList.empty()) return false;
	for (auto& i : friendList) {
		if (i == ign) return true;
	}
	return false;
}

void FriendUtils::removeFromList(std::string name) {
	if (friendList.empty()) return;
	for (int i = 0; i < friendList.size(); i++) {
		if (name == friendList[i]) {
			friendList.erase(friendList.begin() + i);
			break;
		}
	}
}

bool FriendUtils::isInList(std::string name) {
	if (friendList.empty()) return false;
	for (std::string i : friendList) {
		if (i == name) return true;
	}
	return false;
}
