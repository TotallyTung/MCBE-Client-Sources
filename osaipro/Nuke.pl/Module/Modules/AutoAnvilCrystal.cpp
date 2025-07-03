#include "AutoAnvilCrystal.h"
AutoAnvilCrystal::AutoAnvilCrystal() : IModule(0x0, Category::COMBAT, "AutoAnvilCrystal (Height Selector)") {
	this->registerIntSetting("range", &this->range, this->range, 3, 12);
}

AutoAnvilCrystal::~AutoAnvilCrystal() {
}

const char* AutoAnvilCrystal::getModuleName() {
	return "AutoAnvilCrystal";
}

static std::vector<Entity*> targetListavc;

void findEntityavc(Entity* currentEntityavc, bool isRegularEntity) {
	static auto AutoAnvilCrystalMod = moduleMgr->getModule<AutoAnvilCrystal>();

	if (currentEntityavc == nullptr)
		return;
	if (currentEntityavc == Game.getLocalPlayer())  // Skip Local player
		return;
	if (!Game.getLocalPlayer()->canAttack(currentEntityavc, false))
		return;
	if (!currentEntityavc->isAlive())
		return;

	if (currentEntityavc->getEntityTypeId() != 71)
		return;
	float dist = (*currentEntityavc->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < AutoAnvilCrystalMod->range) {
		targetListavc.push_back(currentEntityavc);
	}
}

bool AutoAnvilCrystal::tryAutoAnvilCrystal(Vec3 AutoAnvilCrystal) {
	AutoAnvilCrystal = AutoAnvilCrystal.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(AutoAnvilCrystal));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(AutoAnvilCrystal);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
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
int PlacedCount;
void AutoAnvilCrystal::onTick(GameMode* gm) {
	targetListavc.clear();
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	Game.forEachEntity(findEntityavc);
	if (!targetListavc.empty()) {
		for (Entity* tgt : targetListavc) {
			Vec3 PillarBase = tgt->getPos()->floor();
			int blockID = Game.getLocalPlayer()->region->getBlock(PillarBase)->blockLegacy->blockId;
			if (moduleMgr->AutoDesync()) return; 
			int slotab = 0;
			if (blockID != 145) {
				PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
				Inventory* inv = supplies->inventory;
				slotab = supplies->selectedHotbarSlot;
				for (int n = 0; n < 36; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 49) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y - 1, PillarBase.z - 2));
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y,     PillarBase.z - 2));
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y + 1, PillarBase.z - 2));
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y + 2, PillarBase.z - 2));
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y + 2, PillarBase.z - 1));
				if (moduleMgr->getModule<Silent>()->isEnabled())
					supplies->selectedHotbarSlot = slotab;
				for (int n = 0; n < 36; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 145) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
				tryAutoAnvilCrystal(Vec3(PillarBase.x, PillarBase.y + 2, PillarBase.z));
				if (moduleMgr->getModule<Silent>()->isEnabled())
					supplies->selectedHotbarSlot = slotab;
			};
		};
	}
}

void AutoAnvilCrystal::onLevelRender() {
	if (Game.getLocalPlayer() == nullptr) return;
	if (targetListavc.empty()) return;
	for (Entity* tgt : targetListavc) {
		Vec3 AnvilPositon = tgt->eyePos0;
		AnvilPositon.y -= tgt->height - 4;
		DrawUtils::setColor(0.6f, 0.f, 1.f, 0.25f);
		float offset = 0.46;
		Vec3 RenderPos = Vec3(AnvilPositon.x - offset, AnvilPositon.y, AnvilPositon.z - offset);
		DrawUtils::drawBox3dFilled(RenderPos, RenderPos.add(1.f), 1.f, false, true);
	}
}

void AutoAnvilCrystal::onJoiningServer() {
	targetListavc.clear();
}