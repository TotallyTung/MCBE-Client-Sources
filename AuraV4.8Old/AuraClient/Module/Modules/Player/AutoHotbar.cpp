#include "AutoHotbar.h"

using namespace std;
AutoHotbar::AutoHotbar() : IModule(0, Category::PLAYER, "Automatically replenishes and sorts items in hotbar")
{
	registerBoolSetting("Sword", &Swordhb, Swordhb);
	registerBoolSetting("Pickaxe", &Pickaxehb, Pickaxehb);
	registerBoolSetting("Trident", &tridenthb, tridenthb);
	registerBoolSetting("EXP", &exphb, exphb);
	registerBoolSetting("Potions", &potionshb, potionshb);
	registerBoolSetting("Obsidian", &obsidianhb, obsidianhb);
	registerBoolSetting("Anvil", &anvilhb, anvilhb);
	registerBoolSetting("Crystal", &crystalhb, crystalhb);
	registerBoolSetting("Gapple", &gapplehb, gapplehb);
	registerBoolSetting("Beds", &bedhb, bedhb);
	registerBoolSetting("Totem", &tothb, tothb);
	registerIntSetting("Sword slot", &SwordSlot, SwordSlot, 1, 9);
	registerIntSetting("Trident slot", &tridentSlot, tridentSlot, 1, 9);
	registerIntSetting("EXP slot", &expSlot, expSlot, 1, 9);
	registerIntSetting("Pickaxe slot", &PickaxeSlot, PickaxeSlot, 1, 9);
	registerIntSetting("Obsidian slot", &obsidianSlot, obsidianSlot, 1, 9);
	registerIntSetting("Potion slot", &potionSlot, potionSlot, 1, 9);
	registerIntSetting("Anvil slot", &anvilSlot, anvilSlot, 1, 9);
	registerIntSetting("Crystal slot", &crystalSlot, crystalSlot, 1, 9);
	registerIntSetting("Gapple slot", &gappleSlot, gappleSlot, 1, 9);
	registerIntSetting("Bed slot", &bedSlot, bedSlot, 1, 9);
	registerIntSetting("Totem slot", &totSlot, totSlot, 1, 9);
	registerBoolSetting("No delay", &Nodelay, Nodelay);
}
const char* AutoHotbar::getRawModuleName()
{
	return "Replenish";
}

const char* AutoHotbar::getModuleName()
{
	name = string("Replenish");
	if (Nodelay)
	{
		name = string("Replenish") + string(GRAY) + string("NoDelay");
	}
	return name.c_str();
}

void AutoHotbar::onTick(C_GameMode* gm)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int realgappleSlot = gappleSlot - 1;
	int realtridentSlot = tridentSlot - 1;
	int realexpSlot = expSlot - 1;
	int realobsidianSlot = obsidianSlot - 1;
	int realcrystalSlot = crystalSlot - 1;
	int realPotionSlot = potionSlot - 1;
	int realanvilSlot = anvilSlot - 1;
	int realswordSlot = SwordSlot - 1;
	int realpickaxeSlot = PickaxeSlot - 1;
	int realbedSlot = bedSlot - 1;
	int realtotSlot = totSlot - 1;

	dasword = false;
	datrident = false;
	daexp = false;
	dapickaxe = false;
	daob = false;
	daanvil = false;
	dacry = false;
	dapot = false;
	dagapple = false;
	dabed = false;
	datot = false;

	for (int n = 0; n < 36; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (Swordhb)
		{
			if (!dasword)
			{
				C_ItemStack* obsidianitem = inv->getItemStack(realswordSlot);
				if (obsidianitem->item != nullptr)
				{
					if (obsidianitem->getItem()->itemId != 316)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 316)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realswordSlot);
								}
								else
								{
									inv->setItem(realswordSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dasword = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 316)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realswordSlot);
							}
							else
							{
								inv->setItem(realswordSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dasword = true;
						}
					}
				}
			}
		}
		if (Pickaxehb)
		{
			if (!dapickaxe)
			{
				C_ItemStack* obsidianitem = inv->getItemStack(realpickaxeSlot);
				if (obsidianitem->item != nullptr)
				{
					if (obsidianitem->getItem()->itemId != 318)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 318)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realpickaxeSlot);
								}
								else
								{
									inv->setItem(realpickaxeSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dapickaxe = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 318)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realpickaxeSlot);
							}
							else
							{
								inv->setItem(realpickaxeSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dapickaxe = true;
						}
					}
				}
			}
		}
		if (obsidianhb)
		{
			if (!daob)
			{
				C_ItemStack* obsidianitem = inv->getItemStack(realobsidianSlot);
				if (obsidianitem->item != nullptr)
				{
					if (obsidianitem->getItem()->itemId != 49)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 49)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realobsidianSlot);
								}
								else
								{
									inv->setItem(realobsidianSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								// inv->swapSlots(n, realobsidianSlot);
								daob = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 49)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realobsidianSlot);
							}
							else
							{
								inv->setItem(realobsidianSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							// inv->swapSlots(n, realobsidianSlot);
							daob = true;
						}
					}
				}
			}
		}
		if (anvilhb)
		{
			if (!daanvil)
			{
				C_ItemStack* anvilitem = inv->getItemStack(realanvilSlot);
				if (anvilitem->item != nullptr)
				{
					if (anvilitem->getItem()->itemId != 145)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 145)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realanvilSlot);
								}
								else
								{
									inv->setItem(realanvilSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								// inv->swapSlots(n, realobsidianSlot);
								daanvil = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 145)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realanvilSlot);
							}
							else
							{
								inv->setItem(realanvilSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							// inv->swapSlots(n, realobsidianSlot);
							daanvil = true;
						}
					}
				}
			}
		}
		if (crystalhb)
		{
			if (!dacry)
			{
				C_ItemStack* crystalitem = inv->getItemStack(realcrystalSlot);
				if (crystalitem->item != nullptr)
				{
					if (crystalitem->getItem()->itemId != 637)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 637)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realcrystalSlot);
								}
								else
								{
									inv->setItem(realcrystalSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dacry = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 637)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realcrystalSlot);
							}
							else
							{
								inv->setItem(realcrystalSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dacry = true;
						}
					}
				}
			}
		}
		if (potionshb)

		{
			if (!dapot)
			{
				C_ItemStack* potionitem = inv->getItemStack(realPotionSlot);
				if (potionitem->item != nullptr)
				{
					if (potionitem->getItem()->itemId != 426)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 426)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realPotionSlot);
								}
								else
								{
									inv->setItem(realPotionSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dapot = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 426)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realPotionSlot);
							}
							else
							{
								inv->setItem(realPotionSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dapot = true;
						}
					}
				}
			}
		}
		if (tridenthb)
		{
			if (!datrident)
			{
				C_ItemStack* tridentitem = inv->getItemStack(realtridentSlot);
				if (tridentitem->item != nullptr)
				{
					if (tridentitem->getItem()->itemId != 546)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 546)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realtridentSlot);
								}
								else
								{
									inv->setItem(realtridentSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								datrident = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 546)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realtridentSlot);
							}
							else
							{
								inv->setItem(realtridentSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							datrident = true;
						}
					}
				}
			}
		}
		if (exphb)
		{
			if (!daexp)
			{
				C_ItemStack* expitem = inv->getItemStack(realexpSlot);
				if (expitem->item != nullptr)
				{
					if (expitem->getItem()->itemId != 508) // TODO: Find ItemIDS
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 508) // TODO: Find ItemIDS
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realexpSlot);
								}
								else
								{
									inv->setItem(realexpSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								daexp = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 508) // TODO: Find ItemIDS
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realexpSlot);
							}
							else
							{
								inv->setItem(realexpSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							daexp = true;
						}
					}
				}
			}
		}
		if (bedhb)
		{
			if (!dabed)
			{
				C_ItemStack* beditem = inv->getItemStack(realbedSlot);
				if (beditem->item != nullptr)
				{
					if (beditem->getItem()->itemId != 26)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 26)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realbedSlot);
								}
								else
								{
									inv->setItem(realbedSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dabed = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 26) // TODO: Find ItemIDS
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realbedSlot);
							}
							else
							{
								inv->setItem(realbedSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dabed = true;
						}
					}
				}
			}
		}
		if (gapplehb)
		{
			if (!dagapple)
			{
				C_ItemStack* gappleitem = inv->getItemStack(realgappleSlot);
				if (gappleitem->item != nullptr)
				{
					if (gappleitem->getItem()->itemId != 259)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 259)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realgappleSlot);
								}
								else
								{
									inv->setItem(realgappleSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dagapple = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 259)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realgappleSlot);
							}
							else
							{
								inv->setItem(realgappleSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dagapple = true;
						}
					}
				}
			}
		}
		if (tothb)
		{
			if (!datot)
			{
				C_ItemStack* totemitem = inv->getItemStack(realtotSlot);
				if (totemitem->item != nullptr)
				{
					if (totemitem->getItem()->itemId != 568)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 568)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realtotSlot);
								}
								else
								{
									inv->setItem(realtotSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								datot = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr)
					{
						if (stack->getItem()->itemId == 568)
						{
							if (!Nodelay)
							{
								inv->swapSlots(n, realtotSlot);
							}
							else
							{
								inv->setItem(realtotSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							datot = true;
						}
					}
				}
			}
		}
	}
}
