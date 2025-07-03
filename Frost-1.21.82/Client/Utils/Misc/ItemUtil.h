#pragma once

class ItemUtil
{
public:
	static constexpr std::array<const char*, 5> blacklistedBlocks = {
		"netherreactor",
		"boombox",
		"lilypad",
		"torch",
		"fence"
	};

	static int getAllPlaceables(bool hotbarOnly)
	{
		auto player = InstanceManager::getLocalPlayer();
		int placeables = 0;

		for (int i = 0; i < 36; i++)
		{
			ItemStack* stack = player->getSupplies()->getInventory()->getItemStack(i);
			if (!stack->mItem) continue;
			Item* item = stack->getItem();
			if (hotbarOnly && i > 8) continue;
			if (stack->mBlock)
			{
				// If the string contains any of the blacklisted blocks, skip it (compare using StringUtils::containsIgnoreCase
				bool skip = false;

				for (const auto& blacklistedBlock : blacklistedBlocks)
				{
					if (item->nameContains(blacklistedBlock))
					{
						skip = true;
						break;
					}
				}
				if (skip) continue;


				placeables += stack->mCount;
			}
		}

		return placeables;
	}

	static int getPlaceableItem(bool hotbarOnly, bool prioHighest)
	{
		auto player = InstanceManager::getLocalPlayer();
		if (!player) return -1;
		auto supplies = player->getSupplies();

		int slot = -1;
		// slot, count
		std::map<int, int> placeables;
		for (int i = 0; i < 36; i++)
		{
			ItemStack* stack = supplies->getInventory()->getItemStack(i);
			if (!stack->mItem) continue;
			Item* item = stack->getItem();
			if (hotbarOnly && i > 8) continue;
			if (stack->mBlock)
			{
				// If the string contains any of the blacklisted blocks, skip it (compare using StringUtils::containsIgnoreCase
				bool skip = false;

				for (const auto& blacklistedBlock : blacklistedBlocks)
				{
					if (item->nameContains(blacklistedBlock))
					{
						skip = true;
						break;
					}
				}
				if (skip) continue;

				Block* block = stack->mBlock;

				if (!prioHighest)
				{
					slot = i;
					break;
				}

				placeables[i] = stack->mCount;
			}
		}

		if (prioHighest)
		{
			int highest = 0;
			for (const auto& [pSlot, count] : placeables)
			{
				if (count > highest)
				{
					highest = count;
					slot = pSlot;
				}
			}
		}

		return slot;
	}

	static int getNonSolidBlock(bool hotbarOnly)
	{
		auto player = InstanceManager::getLocalPlayer();
		if (!player) return -1;

		int slot = -1;

		for (int i = 0; i < 36; i++)
		{
			ItemStack* stack = player->getSupplies()->getInventory()->getItemStack(i);
			if (!stack->mItem) continue;
			if (hotbarOnly && i > 8) break;
			if (stack->mBlock && !stack->mBlock->getLegacyBlock()->isSolid()) {
				slot = i;
				break;
			}
		}

		return slot;
	}

	static int getBestBreakingTool(Block* block, bool hotbarOnly)
	{
		auto player = InstanceManager::getLocalPlayer();
		if (!player) return -1;

		auto supplies = player->getSupplies();
		auto container = supplies->getInventory();

		int bestSlot = supplies->mSelectedSlot;
		float bestSpeed = 0.0f;

		for (int i = 0; i < 36; i++)
		{
			auto item = container->getItemStack(i);
			if (!item->mItem) continue;

			float speed = getDestroySpeed(i, block);
			if (speed > bestSpeed)
			{
				bestSpeed = speed;
				bestSlot = i;
			}

			if (hotbarOnly && i > 8) break;
		}

		return bestSlot;
	}

	static float getDestroySpeed(const int slot, const Block* block)
	{
		float result = 1.0f;
		auto player = InstanceManager::getLocalPlayer();
		if (!player) return result;

		int oldSlot = player->getSupplies()->mSelectedSlot;
		player->getSupplies()->mSelectedSlot = slot;
		result = player->getGameMode()->getDestroyRate(*block);
		player->getSupplies()->mSelectedSlot = oldSlot;

		return result;
	}

	static int getItem(ItemType type, bool hotbarOnly)
	{
		auto player = InstanceManager::getLocalPlayer();
		auto supplies = player->getSupplies();
		auto container = supplies->getInventory();

		for (int i = 0; i < 36; i++)
		{
			auto item = container->getItemStack(i);
			if (!item->mItem) continue;

			if (hotbarOnly && i > 8) break;

			if (item->getItem()->getItemType() == type)
			{
				return i;
			}
		}

		return -1;
	}
};