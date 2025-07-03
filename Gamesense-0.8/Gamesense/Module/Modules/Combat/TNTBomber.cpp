#include "TNTBomber.h"

#include "../../../../SDK/Tag.h"
int tickIgnite = 0;
int tickPlace = 0;
TNTBomber::TNTBomber() : IModule(VK_NUMPAD0, Category::COMBAT, "Spams TNTs in peoples heads") {
	registerEnumSetting("Swap", &swap, 0);
	swap.addEntry("Regular", 0);
	swap.addEntry("Silent", 1);
	this->registerIntSetting("Range", &this->range, this->range, 3, 10);
	this->registerIntSetting("Place Delay", &this->placedelay, this->placedelay, 0, 10);
	this->registerIntSetting("Ignite delay", &this->ignitedelay, this->ignitedelay, 0, 10);
	this->registerBoolSetting("Airplace", &this->airplace, this->airplace);
}
TNTBomber::~TNTBomber() {
}
const char* TNTBomber::getModuleName() {
	return ("TNTBomb");
}

static std::vector<C_Entity*> targetList;

void TNTBomber::onEnable() {
	targetList.clear();
}

bool findEntByC(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;  // falling block
	if (curEnt->getEntityTypeId() == 64) return false;  // item
	if (curEnt->getEntityTypeId() == 69) return false;  // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!TargetUtil::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<TNTBomber>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}

bool placeBloKKK(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
	}
	return false;
}

void igniteTNT(vec3_t ignite) {
	ignite = ignite.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(ignite));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(ignite);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = g_Data.getLocalPlayer()->region->getBlock(ignite)->toLegacy()->blockId == 46;
			if (Y) {
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return;
		}
	}
	return;
}

void findTNTNormal() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 46) {
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void findTNTSilent() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 46)
			{
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
void findSwordSilent() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			float currentDamage = stack->getAttackingDamageWithEnchants();
			int fireAspect = stack->getEnchantValue(21);
			if (currentDamage > damage)
			{
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
void findSwordNormal() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 89)
		return;
	float damage = 0;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			int fireAspect = stack->getEnchantValue(21);
			if (currentDamage > damage) {
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void TNTBomber::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	targetList.clear();
	g_Data.forEachEntity(findEntByC);

	int place = 0;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	tntSlot = supplies->selectedHotbarSlot;
	tickIgnite++;
	tickPlace++;
	if (!targetList.empty()) {
		if (swap.getSelectedValue() == 0)
			findTNTNormal();
		if (swap.getSelectedValue() == 1)
			findTNTSilent();
		vec3_t enemyLoc = (targetList[0]->eyePos0).floor();
		neckBreaker = enemyLoc;
		neckBreaker.y += targetList[0]->height;


		if (g_Data.getLocalPlayer()->region->getBlock(neckBreaker)->toLegacy()->blockId == 0) {
			if (tickPlace >= placedelay) {
				if (airplace)
					placeBloKKK(neckBreaker);
				else
					gm->buildBlock(&vec3_ti(neckBreaker), 0);
			}
		}
		if (swap.getSelectedValue() == 0)
			findSwordNormal();
		if (swap.getSelectedValue() == 1)
			findSwordSilent();
		if (tickIgnite >= ignitedelay)
			gm->buildBlock(&vec3_ti(neckBreaker), 0);
		if (swap.getSelectedValue() == 1)
			supplies->selectedHotbarSlot = tntSlot;
		tickIgnite = 0;
		tickPlace = 0;
	}
	return;
}

void TNTBomber::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TNTBomber::onDisable() {
}