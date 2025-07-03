#pragma once

class ItemStack // 182
{
public:
	Item* getItem() // 0x8
	{
		return *(Item**)((uintptr_t)(this) + 0x8);
	}
	std::string getNameId()
	{
		return (std::string)((TextHolder)((uintptr_t)this + 0xC0)).getText();
	}
	bool isBlockType() // *0x8
	{
		return *(uintptr_t**)((uintptr_t)(this) + 0x18) != nullptr;
	}
	bool isWeapon(void) {
		if (getNameId().find("sword") != std::string::npos) return true;
		// if (name.find("axe") != std::string::npos) return true;
		return false;
	}

	Item* item;
	bool isBlock;
	uint8_t count;
};