#include "AnchorAura.h"
AnchorAura::AnchorAura() : IModule('f', Category::COMBAT, "Spams Anchors in peoples heads") {
	this->registerIntSetting("Range", &this->range, this->range, 3, 10);
}
AnchorAura::~AnchorAura() {
}
const char* AnchorAura::getModuleName() {
	return ("AnchorAura");
}
std::vector<Entity*> targetListAnchor;
bool findEntity(Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (!Game.getLocalPlayer()->canAttack(curEnt, false)) return false;
	if (!Game.getLocalPlayer()->isAlive() || !curEnt->isAlive()) return false;
	if (curEnt->getEntityTypeId() != 319) return false;
	if (curEnt->isLocalPlayer()) return false;
	if (FriendsManager::findFriend(curEnt->getNameTag()->getText())) return false;
	float dist = (*curEnt->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<AnchorAura>()->range) {
		targetListAnchor.push_back(curEnt);
		return true;
	}
	return false;
}

bool AnchorbuildBlock(Vec3 blkPlacement) {
	blkPlacement = blkPlacement.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blkPlacement));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blkPlacement);

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
int getbid(Vec3 pos) {
	return Game.getLocalPlayer()->region->getBlock(Vec3i(pos))->toLegacy()->blockId;
}
std::vector<Vec3> CalcAnchorPosition(Entity* plr, Entity* target) {
	std::vector<Vec3> Anchor;
	std::vector<std::pair<Vec3, int>> SurroundedCheck;
	std::vector<int> SideCheck;
	Vec3 MyPosition = *plr->getPos();
	Vec3 targetPosition = target->getPos()->floor(); targetPosition.y -= 1;
	Anchor.clear();
	SurroundedCheck.clear();
	Vec3 Pos1 = Vec3(targetPosition.x, targetPosition.y, targetPosition.z + 1);
	Vec3 Pos2 = Vec3(targetPosition.x, targetPosition.y, targetPosition.z - 1);
	Vec3 Pos3 = Vec3(targetPosition.x + 1, targetPosition.y, targetPosition.z);
	Vec3 Pos4 = Vec3(targetPosition.x - 1, targetPosition.y, targetPosition.z);

	Vec3 Pos5 = Vec3(targetPosition.x, targetPosition.y + 2, targetPosition.z + 1);
	Vec3 Pos6 = Vec3(targetPosition.x, targetPosition.y + 2, targetPosition.z - 1);
	Vec3 Pos7 = Vec3(targetPosition.x + 1, targetPosition.y + 2, targetPosition.z);
	Vec3 Pos8 = Vec3(targetPosition.x - 1, targetPosition.y + 2, targetPosition.z);
	
	Vec3 TargetAbove = Vec3(targetPosition.x - 1, targetPosition.y + 2, targetPosition.z);
	SurroundedCheck.push_back(std::make_pair(Pos1, getbid(Pos1)));
	SurroundedCheck.push_back(std::make_pair(Pos2, getbid(Pos2)));
	SurroundedCheck.push_back(std::make_pair(Pos3, getbid(Pos3)));
	SurroundedCheck.push_back(std::make_pair(Pos4, getbid(Pos4)));

	SideCheck.push_back(getbid(Pos5));
	SideCheck.push_back(getbid(Pos6));
	SideCheck.push_back(getbid(Pos7));
	SideCheck.push_back(getbid(Pos8));
	bool sidecheck = false;
	for (auto IDCheck : SideCheck)
		if (IDCheck != 0) sidecheck = true;
	for (auto Pairs : SurroundedCheck)
		if (Pairs.second == 0) Anchor.push_back(Pairs.first);
	if (getbid(TargetAbove) == 0 && sidecheck)
		Anchor.push_back(TargetAbove);
	return Anchor;
};
void AnchorAura::onTick(GameMode* gm) {
	targetListAnchor.clear();
	if (Game.getLocalPlayer() == nullptr) return;
	if (moduleMgr->AutoDesync()) return;
	Game.forEachEntity(findEntity);
	if (targetListAnchor.empty()) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	int slotab = supplies->selectedHotbarSlot;
	if (getbid(CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0]) == 0, 527) {
		for (int n = 0; n < 36; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 4294967024) {
					supplies->selectedHotbarSlot = n;
					break;
				};
			};
		};
		AnchorbuildBlock(CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0]);
		if (moduleMgr->getModule<Silent>()->isEnabled())
			supplies->selectedHotbarSlot = slotab;

	}
	if (getbid(CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0]) == 527) {
		for (int n = 0; n < 36; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 89) {
					supplies->selectedHotbarSlot = n;
					break;
				};
			};
		};
		gm->buildBlock(&Vec3i(CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0]), 0, true);
		if (moduleMgr->getModule<Silent>()->isEnabled())
			supplies->selectedHotbarSlot = slotab;
	}
};

void AnchorAura::onJoiningServer() {
	targetListAnchor.clear();			
}

void AnchorAura::onLevelRender() {
	if (Game.getLocalPlayer() == nullptr) return;
	if (moduleMgr->AutoDesync()) return;
	if (targetListAnchor.empty()) return;
	MC_Color C = moduleMgr->getModule<Colors>()->getColorV2(0);
	DrawUtils::setColor(C.r, C.g, C.b, 0.12f);
	DrawUtils::drawBox3dFilled(CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0], CalcAnchorPosition(Game.getLocalPlayer(), targetListAnchor[0])[0].add(1.f), 1.f, false, true);
}