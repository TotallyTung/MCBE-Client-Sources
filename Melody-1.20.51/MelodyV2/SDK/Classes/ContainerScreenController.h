// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "ScreenController.h"

#include <magic_enum.hpp>

#include <string>

enum class ItemSource
{
	armor_items,
	container_items,
	hotbar_items,
	inventory_items,
	offhand_items
};

inline std::string itemSourceToString(ItemSource source)
{
	std::string_view sourceSV = magic_enum::enum_name(source);
	std::string sourceStr(sourceSV);
	return sourceStr;
}
/*int getEnchantLevel(int enchantId) {
		using func_t = int (__thiscall*)(int, ItemStackBase*);
		static func_t func = (func_t)findSig(Sigs::enchantutils::getenchantlevel);
		return func(enchantId, this);
}*/
class ContainerScreenController : public ScreenController
{
public:
	void _handleAutoPlace(ItemSource source, int slot) {
		std::string sourceStr = itemSourceToString(source);


		using func_t = void(__fastcall*)(ContainerScreenController*, int, std::string&, int);
		static func_t func = (func_t)findSig(Sigs::ContainerScreenController::ContainerScreenController__handleAutoPlace);
		return func(this, 0x7fffffff, sourceStr, slot);
	}

	void _handleDropItem(ItemSource source, int slot)
	{
		std::string sourceStr = itemSourceToString(source);
		int* stack = reinterpret_cast<int*>(_malloca(24));
		memset(stack, 0, 24);
		stack[0] = 1;

		using func_t = void(__fastcall*)(ContainerScreenController*, std::string&, int, int*);
		static func_t func = (func_t)findSig(Sigs::ContainerScreenController::ContainerScreenController__handleDropItem);
		return func(this, sourceStr, slot, stack);
	}

	void _handlePlaceAll(ItemSource source, int slot)
	{
		std::string sourceStr = itemSourceToString(source);


		using func_t = void(__fastcall*)(ContainerScreenController*, std::string&, int);
		static func_t func = (func_t)findSig(Sigs::ContainerScreenController::ContainerScreenController__handlePlaceAll);
		return func(this, sourceStr, slot);
	}

	void _handleTakeAll(ItemSource source, int slot)
	{
		std::string sourceStr = itemSourceToString(source);

		using func_t = void(__fastcall*)(ContainerScreenController*, std::string&, int);
		static func_t func = (func_t)findSig(Sigs::ContainerScreenController::ContainerScreenController__handleTakeAll);
		return func(this, sourceStr, slot);
	}
};