#pragma once
class ItemStack;

namespace Containers
{
	static std::string Inventory = "inventory_items";
	static std::string Hotbar = "hotbar_items";
	static std::string Container = "container_items";
	static std::string Armor = "armor_items";
};

class ContainerScreenController { // indexes cuz im gonna make them methods that aren't virtual
public:
	// handleAutoPlace
	void handleTakeAll(std::string containerName, int slots) {
		//static void* shiftClick = Memory::findSig(xorstr_("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 8B E1 49 8B F0 44 8B FA 89 54 24 40 48 8B F9 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 8F ? ? ? ? 48 85 C9 74 6B E8 ? ? ? ? 84 C0 74 62 48 8D 8F ? ? ? ? 8B 91 ? ? ? ? 39 91 ? ? ? ? 74 13 44 89 64 24 ? 4C 8B CE"));
		static void* shiftClick = Memory::findSig(xorstr_("E8 ? ? ? ? 49 8B 46 ? C6 80 ? ? ? ? ? 49 8B 5E"));

		/*
		Memory::CallFunc<void, ContainerScreenController*, uintptr_t, TextHolder, int>(
			shiftClick, this, 0x7FFFFFFF, containerName, slots);*/
		Memory::CallFunc<void, ContainerScreenController*, TextHolder, int32_t>(
			shiftClick, this, TextHolder(containerName), slots);
	}

	void handlePlaceAll(std::string containerName, int32_t slot) {
		Memory::CallVFunc<56, void, TextHolder, int32_t>(this, TextHolder(containerName), slot);
	}

	// handleSwap
	void handleSwap(std::string name1, int slot1, std::string name2, int slot2) {
		this->handleTakeAll(name1, slot1);
		this->handlePlaceAll(name2, slot2);
	}

	void tryExit() {
		Memory::CallVFunc<12, void>(this);
	}

	void closeContainer() {
		//this->leaveScreen();
		//this->canExit();
		this->tryExit();
	}
};

class ChestScreenController : public ContainerScreenController {
};
