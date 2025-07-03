#include "AutoHotbar.h"

using namespace std;
bool TNThb = true;
int TNTSlot = 7;
AutoHotbar::AutoHotbar() : IModule(0, Category::PLAYER, "Automatically replenishes and sorts items in hotbar")
{
	registerBoolSetting("Sword", &Swordhb, Swordhb);
	registerBoolSetting("Pickaxe", &Pickaxehb, Pickaxehb);
	registerBoolSetting("Obsidian", &obsidianhb, obsidianhb);
	registerBoolSetting("Crystal", &crystalhb, crystalhb);
	registerBoolSetting("Potions", &potionshb, potionshb);
	registerBoolSetting("Beds", &bedhb, bedhb);
	registerBoolSetting("Anvil", &anvilhb, anvilhb);
	registerBoolSetting("TNT", &TNThb, TNThb);
	registerBoolSetting("Gapple", &gapplehb, gapplehb);
	registerIntSetting("Sword slot", &SwordSlot, SwordSlot, 1, 9);
	registerIntSetting("Pickaxe slot", &PickaxeSlot, PickaxeSlot, 1, 9);
	registerIntSetting("Obsidian slot", &obsidianSlot, obsidianSlot, 1, 9);
	registerIntSetting("Crystal slot", &crystalSlot, crystalSlot, 1, 9);
	registerIntSetting("Potion slot", &potionSlot, potionSlot, 1, 9);
	registerIntSetting("Bed slot", &bedSlot, bedSlot, 1, 9);
	registerIntSetting("Anvil slot", &anvilSlot, anvilSlot, 1, 9);
	registerIntSetting("TNT slot", &TNTSlot, TNTSlot, 1, 9);
	registerIntSetting("Gapple slot", &gappleSlot, gappleSlot, 1, 9);
}
const char* AutoHotbar::getModuleName() {
	return ("Replenish");
}
float interval = 0.f;
void AutoHotbar::onJoiningServer() {
	interval = 10.f;
}
void AutoHotbar::onTick(GameMode* gm)
{
	auto player = Game.getLocalPlayer();
	if (player == nullptr)
		return;
	if (interval >= 0.1f) {
		interval -= 0.1f;
		return;
	}
	Nodelay = false;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	int realgappleSlot = gappleSlot - 1;
	int realobsidianSlot = obsidianSlot - 1;
	int realcrystalSlot = crystalSlot - 1;
	int realPotionSlot = potionSlot - 1;
	int realanvilSlot = anvilSlot - 1;
	int realswordSlot = SwordSlot - 1;
	int realpickaxeSlot = PickaxeSlot - 1;
	int realTntSlot = TNTSlot - 1;
	int realbedSlot = bedSlot - 1;

	dasword = false;
	dapickaxe = false;
	daob = false;
	daanvil = false;
	dacry = false;
	dapot = false;
	bool datn = false;
	dagapple = false;
	dabed = false;

	for (int n = 0; n < 36; n++)
	{
		ItemStack* stack = inv->getItemStack(n);
		if (Swordhb)
		{
			if (!dasword)
			{
				ItemStack* obsidianitem = inv->getItemStack(realswordSlot);
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
				ItemStack* obsidianitem = inv->getItemStack(realpickaxeSlot);
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
				ItemStack* obsidianitem = inv->getItemStack(realobsidianSlot);
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
				ItemStack* anvilitem = inv->getItemStack(realanvilSlot);
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
		if (TNThb)
		{
			if (!datn)
			{
				ItemStack* tntitem = inv->getItemStack(realanvilSlot);
				if (tntitem->item != nullptr)
				{
					if (tntitem->getItem()->itemId != 46)
					{
						if (stack->item != nullptr)
						{
							if (stack->getItem()->itemId == 46)
							{
								if (!Nodelay)
								{
									inv->swapSlots(n, realTntSlot);
								}
								else
								{
									inv->setItem(realTntSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								// inv->swapSlots(n, realobsidianSlot);
								datn = true;
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
								inv->swapSlots(n, realTntSlot);
							}
							else
							{
								inv->setItem(realTntSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							// inv->swapSlots(n, realobsidianSlot);
							datn = true;
						}
					}
				}
			}
		}
		if (crystalhb)
		{
			if (!dacry)
			{
				ItemStack* crystalitem = inv->getItemStack(realcrystalSlot);
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
				ItemStack* potionitem = inv->getItemStack(realPotionSlot);
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
		if (bedhb)
		{
			if (!dabed)
			{
				ItemStack* beditem = inv->getItemStack(realbedSlot);
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
				ItemStack* gappleitem = inv->getItemStack(realgappleSlot);
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
	}
}
