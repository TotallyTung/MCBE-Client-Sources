#pragma once

class Item
{
public:
	std::string getNameId()
	{
		return (std::string)((TextHolder)((uintptr_t)this + 0xC0)).getText();
	}
	bool isWeapon(void) {
		if (getNameId().find("sword") != std::string::npos) return true;
		// if (name.find("axe") != std::string::npos) return true;
		return false;
	}
	bool isPickaxe(void) {
		if (name.find("pickaxe") != std::string::npos) return true;
		return false;
	}


	uint16_t maxStackSize;
	std::string name;
};