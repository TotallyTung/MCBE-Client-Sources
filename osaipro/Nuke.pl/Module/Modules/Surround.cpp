#include "Surround.h"

#include "../../../Utils/HMath.h"
inline SettingEnum ViewMode;
Surround::Surround() : IModule(0, Category::COMBAT, "Protect yourself from crystals :)") {
	registerBoolSetting("Render", &this->renderSurround, true);
	registerEnumSetting("ViewMode", &ViewMode, 0);
	ViewMode.addEntry("Outline", 0);
	ViewMode.addEntry("Filled", 1);
	ViewMode.addEntry("Both", 2);
}

Surround::~Surround() {
}

const char* Surround::getModuleName() {
	return "Surround";
}
/*
bool Surround::isHoldMode() {
	return false;
}
*/

void getObby() {
	auto supplies = Game.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}


bool Surround::isSideEmpty(std::vector<Vec3> breh) {
	bool sendBack = false;
	for (auto checkThis : breh) {
		auto playerOn = Game.getLocalPlayer()->getPos();
		auto rndOffPlayerOn = playerOn->floor();
		rndOffPlayerOn.y -= 1.f;

		auto calcSides = rndOffPlayerOn.add(checkThis);
		if (Game.getLocalPlayer()->region->getBlock(Vec3i(calcSides))->toLegacy()->blockId == 0) {
			sendBack = true;
		}
	}
	if (sendBack == true) {
		return true;
	} else {
		return false;
	}
}

void Surround::onMove(MoveInputHandler* hand) {
	if (Game.getLocalPlayer() == nullptr) return;

	if (jumpDisable && hand->isJumping) this->setEnabled(false);
}

bool tryPlace(Vec3 blkPlacement) {
	blkPlacement = blkPlacement.floor();

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
			Game.getGameMode()->buildBlock(&blok, i, true); // if it breaks, then idk
			return true;
		}
	}
	return false;
}

void Surround::onEnable() {
	if (Game.getLocalPlayer() == nullptr) return;
	origSlot = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (!citySwitch)
		getObby();
}

bool doReset = true;
std::vector<Vec3> surrArr;
bool stopSwitch = false;
std::vector<Vec3> blocky;

void Surround::onWorldTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	blocky.clear();
	pn = false;

	// LOCK PLAYER POS
	Vec3 yR = Vec3(floor(Game.getLocalPlayer()->getPos()->x), Game.getLocalPlayer()->getPos()->y, floor(Game.getLocalPlayer()->getPos()->z));
	yR.x += 0.5f;
	yR.z += 0.5f;
	bool dontcenterme = false;
	auto playerPosition = Game.getLocalPlayer()->getPos();
	auto flooredPos = playerPosition->floor();
	flooredPos.y -= 1.f;
	for (auto surroundBlock : regularBlocks) {
		auto mathsBlockPos = flooredPos.add(surroundBlock);
		blocky.push_back(mathsBlockPos);
	}
	bool auau = false;
	for (auto surroundBlock : blocky) {
		auto mathsBlockPos = surroundBlock;

		if (gm->player->getAABB()->intersects(AABB(mathsBlockPos, mathsBlockPos.add(1)))) {
			yR.x += (surroundBlock.x - flooredPos.x) / 2;
			yR.z += (surroundBlock.z - flooredPos.z) / 2;
			for (auto& block : regularBlocks) {
				auto mathsBlockPos1 = mathsBlockPos.add(block);

				if (!gm->player->getAABB()->intersects(AABB(mathsBlockPos1, mathsBlockPos1.add(1))))
				{
					blocky.push_back(mathsBlockPos1);
				}
			}
			dontcenterme = true;
			break;
		}
	}
	int pro = 0;
	if (isSideEmpty(blocky)) {
		pn = true;
		int prev = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		if (citySwitch) getObby();
		for (auto surroundBlock : blocky) {
			auto mathsBlockPos = (surroundBlock);

			if (gm->player->getAABB()->intersects(AABB(mathsBlockPos, mathsBlockPos.add(1)))) {
				blocky.erase(blocky.begin() + pro);
				continue;
			}

			if (!Game.getLocalPlayer()->region->getBlock(mathsBlockPos)->toLegacy()->material->isReplaceable) continue;
			tryPlace(mathsBlockPos);
			++pro;
		}
		Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prev;
	}
}
void Surround::onLevelRender() {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!renderSurround) return;
 	static GameMode* gm = Game.getGameMode();
	MC_Color cc = MC_Color(moduleMgr->getModule<Colors>()->getColorV2().r, moduleMgr->getModule<Colors>()->getColorV2().g, moduleMgr->getModule<Colors>()->getColorV2().b);
	for (auto SurroundBlocky : blocky) {
		if ((SurroundBlocky.y != Game.getLocalPlayer()->getPos()->floor().y - 2 || (SurroundBlocky.x == Game.getLocalPlayer()->getPos()->floor().x && SurroundBlocky.y == Game.getLocalPlayer()->getPos()->floor().y - 2 && SurroundBlocky.z == Game.getLocalPlayer()->getPos()->floor().z)) && Game.getLocalPlayer()->region->getBlock(SurroundBlocky)->toLegacy()->blockId == 49) {
			if (ViewMode.getSelectedValue() == 0 || ViewMode.getSelectedValue() == 2) {
				if (ViewMode.getSelectedValue() == 0) DrawUtils::setColor(cc.r, cc.g, cc.b, 1.f);
				if (ViewMode.getSelectedValue() == 2) DrawUtils::setColor(cc.r, cc.g, cc.b, 0.12f);
				DrawUtils::drawBox3d(SurroundBlocky, SurroundBlocky.add(1.f), 1.f, true);
			}
			if (ViewMode.getSelectedValue() == 1 || ViewMode.getSelectedValue() == 2) {
				DrawUtils::setColor(cc.r, cc.g, cc.b, 0.12f);
				DrawUtils::drawBox3dFilled(SurroundBlocky, SurroundBlocky.add(1.f), 1.f, false, true);
			}
		};
	}
}
