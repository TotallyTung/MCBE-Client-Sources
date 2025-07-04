#include "AntiAnvil.h"

AntiAnvil::AntiAnvil() : IModule(0x0, Category::PLAYER, "Auto break anvil if we are in anvil!!!") {
	//registerBoolSetting("Silent", &silenttd2, silenttd2);
}

const char* AntiAnvil::getModuleName() {
	return ("AntiAnvil");
}

void AntiAnvil::findPickaxeLOL3() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				float currentDamage = stack->getAttackingDamageWithEnchants();
				if (currentDamage > damage) {
					damage = currentDamage;
					slot = n;
				}
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void AntiAnvil::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void AntiAnvil::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;
	
	blockpos3 = plr->getPos()->floor();
	blockpos3.y = (int)blockpos3.y - 0.6f;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockpos3);
	int blockID = (int)block->toLegacy()->blockId;
	slottd2 = supplies->selectedHotbarSlot;
	if (blockID == 145) {
		caidelljthe = true;
		findPickaxeLOL3();
		g_Data.getCGameMode()->destroyBlock(&vec3_ti(blockpos3.x, blockpos3.y, blockpos3.z), 1);
		supplies->selectedHotbarSlot = slottd2;
	}
	//	supplies->selectedHotbarSlot = cc;
	
}

void AntiAnvil::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	
	if (GameData::canUseMoveKeys()) {
		if (!moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
			if (caidelljthe) {
				DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
				DrawUtils::drawBox(vec3_t(blockpos3.toVector3().floor()), vec3_t(blockpos3.toFloatVector().floor().add(1.f)), 0.5f, false);
				caidelljthe = false;
			}
		}
	}
	
}