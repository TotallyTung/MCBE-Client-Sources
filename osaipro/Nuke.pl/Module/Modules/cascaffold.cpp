#include "cascaffold.h"

cascaffold::cascaffold() : IModule(0x0, Category::COMBAT, "Place obsidian to crystal target!!!") {
	registerFloatSetting("Range", &range, range, 3, 10);
	registerBoolSetting("notarget", &notarget, notarget);
}

const char* cascaffold::getModuleName() {
	return ("cascaffold");
}

void cascaffold::findObsi() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {
				Game.getLocalPlayer()->switchHotbar(n);
				break;
			}
		}
	}
	//supplies->selectedHotbarSlot = slot;
}
struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static std::vector<Entity*> targetListcc2;
void findTargetgg(Entity* currentEntity, bool isRegularEntity) {
	if (!Target::isValidTarget(currentEntity)) return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < moduleMgr->getModule<cascaffold>()->range) {
		targetListcc2.push_back(currentEntity);
		sort(targetListcc2.begin(), targetListcc2.end(), CompareTargetEnArray());
	}
}
bool cascaffold::canpla(Vec3i blockPos) {
	if (Game.getLocalPlayer()->getPos()->dist(blockPos.toVec3t()) >= 7) {
		return false;
	}
	else return true;
}

bool cascaffold::tryAutoTrap2(Vec3i blkPlacement) {
	//blkPlacement = blkPlacement.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blkPlacement));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blkPlacement);
		int i = 0;

		static std::vector<Vec3i*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			Game.getGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}
void cascaffold::onTick(GameMode* gm) {
	targetListcc2.clear();
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (moduleMgr->AutoDesync()) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	Game.forEachEntity(findTargetgg);
	int slotab = supplies->selectedHotbarSlot;
	if (!notarget) {
		if (!targetListcc2.empty()) {
			for (auto& a : targetListcc2) {
				Vec3i targetPosi = targetListcc2[0]->getPos()->floor();
				for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
					for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
						findObsi();
						if (canpla(Vec3i(x, (int)targetPosi.y - 1.6f, z))) tryAutoTrap2(Vec3i(x, (int)targetPosi.y - 1.6f, z));
						if (moduleMgr->getModule<Silent>()->isEnabled()) player->switchHotbar(slotab);
					}
				}
			}
		}
	};
	if (notarget) {
		Vec3i targetPosi = Game.getLocalPlayer()->getPos()->floor();
		for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
			for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
				findObsi();
				if (canpla(Vec3i(x, (int)targetPosi.y - 1.6f, z))) tryAutoTrap2(Vec3i(x, (int)targetPosi.y - 1.6f, z));
				if (moduleMgr->getModule<Silent>()->isEnabled()) player->switchHotbar(slotab);
			}
		}
	};
}

void cascaffold::onJoiningServer() {
	targetListcc2.clear();
}