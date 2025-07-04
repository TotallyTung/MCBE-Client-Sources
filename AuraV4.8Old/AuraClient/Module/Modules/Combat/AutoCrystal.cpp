#include "AutoCrystal.h"
#include "../pch.h"

using namespace std;
AutoCrystal::AutoCrystal() : IModule(0, Category::PLAYER, "Auto put things in hotbar") {
	/*
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Inventory", 1);
	*/
	registerBoolSetting("Gapple", &gapplehb, gapplehb);
	registerIntSetting("Gapple slot", &gappleSlot, gappleSlot, 1, 9);

	registerBoolSetting("Obsidian", &obsidianhb, obsidianhb);
	registerIntSetting("Obsidian slot", &obsidianSlot, obsidianSlot, 1, 9);

	registerBoolSetting("Crystal", &crystalhb, crystalhb);
	registerIntSetting("Crystal slot", &crystalSlot, crystalSlot, 1, 9);

	registerBoolSetting("Strength", &strengthhb, strengthhb);
	registerIntSetting("Strength slot", &strengthSlot, strengthSlot, 1, 9);

	registerBoolSetting("No delay", &Nodelay, Nodelay);
}
const char* AutoCrystal::getRawModuleName() {
	return "AutoHotbar";
}

const char* AutoCrystal::getModuleName() {
	name = string("AutoHotbar");
	return name.c_str();
}


void AutoCrystal::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	/*
	if (!player->canOpenContainerScreen() && mode.getSelectedValue() == 0) return;
	if (player->canOpenContainerScreen() && mode.getSelectedValue() == 1) return;
	*/ // Ncc

	/*
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int realslot = crystalSlot - 1;
	C_ItemStack* wtf = inv->getItemStack(realslot);
	if (wtf->item != nullptr) {
		if (wtf->getItem()->itemId != 637) {
			for (int n = 0; n < 36; n++) {
				C_ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 637) {
						inv->swapSlots(n, realslot);
						break;
					}
				}
			}
		}
	}
	else
	{
		for (int n = 0; n < 36; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 637) {
					inv->swapSlots(n, realslot);
					break;
				}
			}
		}
	}
	*/
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int realgappleSlot = gappleSlot - 1;
	int realobsidianSlot = obsidianSlot - 1;
	int realcrystalSlot = crystalSlot - 1;
	int realstrengthSlot = strengthSlot - 1;

	dagapple = false;
	daob = false;
	dacry = false;
	dastrength = false;

	for (int n = 0; n < 36; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (gapplehb) {
			if (!dagapple) {
				C_ItemStack* gappleitem = inv->getItemStack(realgappleSlot);
				if (gappleitem->item != nullptr) {
					if (gappleitem->getItem()->itemId != 259) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 259) {
								if (!Nodelay) {
									inv->swapSlots(n, realgappleSlot);
								}
								else
								{
									inv->setItem(realgappleSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								//inv->swapSlots(n, realgappleSlot);
								dagapple = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 259) {
							if (!Nodelay) {
								inv->swapSlots(n, realgappleSlot);
							}
							else
							{
								inv->setItem(realgappleSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							//inv->swapSlots(n, realgappleSlot);
							dagapple = true;
						}
					}
				}
			}
		}

		if (obsidianhb) {
			if (!daob) {
				C_ItemStack* obsidianitem = inv->getItemStack(realobsidianSlot);
				if (obsidianitem->item != nullptr) {
					if (obsidianitem->getItem()->itemId != 49) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 49) {
								if (!Nodelay) {
									inv->swapSlots(n, realobsidianSlot);
								}
								else
								{
									inv->setItem(realobsidianSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								//inv->swapSlots(n, realobsidianSlot);
								daob = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 49) {
							if (!Nodelay) {
								inv->swapSlots(n, realobsidianSlot);
							}
							else
							{
								inv->setItem(realobsidianSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							//inv->swapSlots(n, realobsidianSlot);
							daob = true;
						}
					}
				}
			}
		}

		if (crystalhb) {
			if (!dacry) {
				C_ItemStack* crystalitem = inv->getItemStack(realcrystalSlot);
				if (crystalitem->item != nullptr) {
					if (crystalitem->getItem()->itemId != 637) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 637) {
								if (!Nodelay) {
									inv->swapSlots(n, realcrystalSlot);
								}
								else
								{
									inv->setItem(realcrystalSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								//inv->setItem(realcrystalSlot, *stack);
								//inv->removeItem(n, n);
								//inv->swapSlots(n, realcrystalSlot);
								dacry = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 637) {
							if (!Nodelay) {
								inv->swapSlots(n, realcrystalSlot);
							}
							else
							{
								inv->setItem(realcrystalSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							//inv->setItem(realcrystalSlot, *stack);
							//inv->removeItem(n, n);
							//inv->swapSlots(n, realcrystalSlot);
							dacry = true;
						}
					}
				}
			}
		}
		if (strengthhb) {
			if (!dastrength) {
				C_ItemStack* strengthitem = inv->getItemStack(realstrengthSlot);
				if (strengthitem->item != nullptr) {
					if (strengthitem->getItem()->itemId != 426) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 426) {
								if (!Nodelay) {
									inv->swapSlots(n, realstrengthSlot);
								}
								else
								{
									inv->setItem(realstrengthSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								//inv->swapSlots(n, realobsidianSlot);
								dastrength = true;
							}
						}
					}
				}
				else
				{
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 145) {
							if (!Nodelay) {
								inv->swapSlots(n, realstrengthSlot);
							}
							else
							{
								inv->setItem(realstrengthSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							//inv->swapSlots(n, realobsidianSlot);
							dastrength = true;
						}
					}
				}
			}
		}

	}
}
