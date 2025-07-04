#include "TargetDown.h"

TargetDown::TargetDown() : IModule(0x0, Category::COMBAT, "Break block below target") {
	registerFloatSetting("Range", &range, range, 3, 10);
	registerBoolSetting("Silent", &silenttd, silenttd);
}

const char* TargetDown::getModuleName() {
	return ("TargetDown");
}

void TargetDown::findPickaxeLOL2() {
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


struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};


static std::vector<C_Entity*> targetListcc;
void findTargetg(C_Entity* currentEntity, bool isRegularEntity) {

	if (!TargetUtil::isValidTarget(currentEntity)) return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < moduleMgr->getModule<TargetDown>()->range) {
		targetListcc.push_back(currentEntity);
		sort(targetListcc.begin(), targetListcc.end(), CompareTargetEnArray());
	}
}

void TargetDown::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListcc.clear();
//	blockpos = pointing->block;

	g_Data.forEachEntity(findTargetg);
	if (!targetListcc.empty()) {
		for (auto& a : targetListcc) {
			blockpos2 = targetListcc[0]->getPos()->floor();
			//blockpos2.x = (int)blockpos2.x - 1;
			blockpos2.y = (int)blockpos2.y - 1.6f;
			//blockpos2.z = (int)blockpos2.z - 1;
		}
	}
	else
	{
		clientMessageF("%s%s[%sTargetDown%s] %sNo target", BOLD, GRAY, GOLD, GRAY, RED);
		this->setEnabled(false);
	}
}

void TargetDown::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	slottd = supplies->selectedHotbarSlot;
	if (g_Data.getLocalPlayer()->region->getBlock(blockpos2)->blockLegacy->blockId != 0) {
		if (silenttd) {
			findPickaxeLOL2();
		}
		g_Data.getCGameMode()->destroyBlock(&vec3_ti(blockpos2.x, blockpos2.y, blockpos2.z), 1);
		if (silenttd) {
			supplies->selectedHotbarSlot = slottd;
		}
	}
	//	supplies->selectedHotbarSlot = cc;
}

void TargetDown::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (GameData::canUseMoveKeys()) {
		if (!moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
			DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
			DrawUtils::drawBox(vec3_t(blockpos2.toVector3().floor()), vec3_t(blockpos2.toFloatVector().floor().add(1.f)), 0.5f, false);
		}
	}
}