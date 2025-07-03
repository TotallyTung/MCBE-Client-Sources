#pragma once
#include "../../SDK/Game.h"

class FriendUtils {
public:
	static bool isInList(Actor* actor);
	static bool isInList(std::string name);
	static void addToList(Actor* actor);
	static void addToList(std::string name);
	static void removeFromList(Actor* actor);
	static void removeFromList(std::string name);
	static void loadList(std::vector<std::string> list);
	static void clearList();
	static std::vector<std::string> getList();
};