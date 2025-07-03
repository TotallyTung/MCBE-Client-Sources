#include "AutoCrystal.h"
#include "../../Utils/ImGuiUtils.h"
#include "../ModuleManager.h"
std::string targetname = "None";
int stoph = 6;
AutoCrystal::AutoCrystal() : IModule(0x0, Category::COMBAT, "Like CrystalAura lol") {
	registerFloatSetting("Target Range", &targetRange, targetRange, 5.f, 20.f);
	//Place
	registerBoolSetting("Auto Place", &autoPlace, autoPlace);
	registerFloatSetting("Place Range", &placeRange, placeRange, 3.f, 10.f);
	registerFloatSetting("Max Crystal Range", &crystalRange, crystalRange, 1.f, 6.f);
	registerFloatSetting("Min Place Dame", &minPlaceDame, minPlaceDame, 1.f, 20.f);
	registerFloatSetting("Max Place Dame", &maxPlaceDame, maxPlaceDame, 1.f, 20.f);
	registerIntSetting("MultiPlace", &multiPlace, multiPlace, 1, 5);
	registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 20);
	//Break
	registerBoolSetting("Auto Break", &autoBreak, autoBreak);
	registerFloatSetting("Break Range", &breakRange, breakRange, 3.f, 10.f);
	registerFloatSetting("Min Break Dame", &minBreakDame, minBreakDame, 1.f, 20.f);
	registerFloatSetting("Max Break Dame", &maxBreakDame, maxBreakDame, 1.f, 20.f);
	registerIntSetting("Break Delay", &breakDelay, breakDelay, 0, 20);
	registerBoolSetting("ID Predict", &idPredict, idPredict);
	registerIntSetting("Packets", &packets, packets, 1, 30);
	registerIntSetting("Send Delay", &sendDelay, sendDelay, 0, 20);
	//Switch
	switchMode.addEntry("None", 0);
	switchMode.addEntry("Normal", 1);
	switchMode.addEntry("Spoof", 2);
	registerEnumSetting("Switch", &switchMode, 0);
	registerBoolSetting("Stop On Eat", &stopOnEat, stopOnEat);
	registerIntSetting("Stop Health", &stoph, stoph, 8, 20);
};

const char* AutoCrystal::getModuleName() {
	std::string awa = "AutoCrystal" + Utils::getClientName();
	return awa.c_str();
}

const char* AutoCrystal::getModName() {
	return targetname.c_str();
}

void AutoCrystal::onEnable() {
}
void AutoCrystal::onDisable() {
}

bool AutoCrystal::sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2) {
	return a1.TgDameTake > a2.TgDameTake;
}

bool AutoCrystal::sortEntityByDist(Entity* a1, Entity* a2) {
	Vec3 localPlayerPos = Game.getLocalPlayer()->getHumanPos();
	return a1->getHumanPos().dist(localPlayerPos) < a2->getHumanPos().dist(localPlayerPos);
}

bool AutoCrystal::isHoldingCrystal() {
	return (Game.getLocalPlayer()->getSelectedItemId() == 637);
}

bool AutoCrystal::canPlaceCrystal(BlockSource* blockSource, const Vec3i& placePos, Entity* tg) {
	Block* block = blockSource->getBlock(placePos);
	Block* upper1 = blockSource->getBlock(placePos.add(0, 1, 0));
	Block* upper2 = blockSource->getBlock(placePos.add(0, 2, 0));
	if (block == nullptr || upper1 == nullptr || upper2 == nullptr) return false;
	if (!(block->blockLegacy->blockId == 49 || block->blockLegacy->blockId == 7)) return false;
	if (upper1->blockLegacy->blockId != 0) return false;
	if (upper2->blockLegacy->blockId != 0) return false;
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer->getHumanPos().dist(placePos.toVec3t().add(0.5f)) > placeRange) return false;

	Vec3 _Pos = placePos.toVec3t();
	_Pos.y += 1.f;
	if (tg->aabb.intersects(AABB(_Pos, _Pos.add(1.f)))) return false;
	for (Entity* ent : entityList) {
		if (ent->getEntityTypeId() == 71) continue;
		AABB targetAABB = ent->aabb;

		if (ent->getEntityTypeId() == 319) {
			targetAABB.lower = targetAABB.lower.sub(0.1f, 0.f, 0.1f);
			targetAABB.upper = targetAABB.upper.add(0.1f, 0.f, 0.1f);
		}

		if (targetAABB.intersects(AABB(_Pos, _Pos.add(1.f)))) return false;
	}
	return true;
}

int AutoCrystal::getCrystalSlot(LocalPlayer* localPlayer) {
	PlayerInventoryProxy* supplies = localPlayer->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int i = 0; i < 9; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->isValid()) {
			if (stack->getItem()->itemId == 637) {
				return i;
			}
		}
	}
	return -1;
}

void AutoCrystal::generatePlacement(LocalPlayer* localPlayer, Entity* target) {
	BlockSource* region = localPlayer->region;
	int radius = (int)(crystalRange);
	for (int x = -radius; x <= radius; x++) {
		for (int y = -3; y <= 3; y++) {
			for (int z = -radius; z <= radius; z++) {
				Vec3 targetPos = target->getHumanPos();
				Vec3i blockPos((int)targetPos.x + x, (int)targetPos.y + y, (int)targetPos.z + z);
				if (canPlaceCrystal(region, blockPos, target)) {
					CrystalPlacements placement(blockPos, localPlayer, target);
					if (placement.LpDameTake < maxPlaceDame && placement.TgDameTake >= minPlaceDame) placeList.push_back(placement);
				}
			}
		}
	}
	std::sort(placeList.begin(), placeList.end(), AutoCrystal::sortCrystalByTargetDame);
}

void AutoCrystal::placeCrystal(GameMode* gm) {
	if (placeList.empty()) return;
	if (!isHoldingCrystal()) return;
	if (placeDelayTick >= placeDelay) {
		int placed = 0;
		for (CrystalPlacements placement : placeList) {
			gm->buildBlock(&placement.placePos, random(0, 5), false);
			placed++;
			if (placed >= multiPlace) break;
		}
		placeDelayTick = 0;
	}
	else {
		placeDelayTick++;
	}
}

void AutoCrystal::getCrystalActorList(LocalPlayer* localPlayer, Entity* target) {
	for (Entity* ent : entityList) {
		if (ent->getEntityTypeId() != 71) continue;
		if (*(int*)ent->getUniqueId() > highestID) highestID = *(int*)ent->getUniqueId();
		if (ent->eyePos0.dist(localPlayer->getHumanPos()) > breakRange) continue;
		CrystalBreaker cBreaker(ent, localPlayer, target);
		if (cBreaker.LpDameTake < maxBreakDame && cBreaker.TgDameTake >= minBreakDame) breakList.push_back(cBreaker);
	}
	std::sort(breakList.begin(), breakList.end(), AutoCrystal::sortCrystalByTargetDame);
}

void AutoCrystal::breakCrystal(GameMode* gm) {
	if (!autoBreak) return;
	if (!breakList.empty()) {
		if (breakDelayTick >= breakDelay) {
			highestID = *(int*)breakList[0].crystalActor->getUniqueId();
			gm->attack(breakList[0].crystalActor);
			breakDelayTick = 0;
		}
		else {
			breakDelayTick++;
		}
	}
}

void AutoCrystal::breakIdPredictCrystal(GameMode* gm) {
	if (!autoBreak) return;
	if (placeList.empty()) return;
	if (sendDelayTick >= sendDelay) {
		shouldChangeID = true;
		for (int i = 0; i < packets; i++) {
			gm->attack(placeList[0].targetActor);
			highestID++;
		}
		highestID -= packets;
		shouldChangeID = false;
		sendDelayTick = 0;
	}
	else {
		sendDelayTick++;
	}
}

void AutoCrystal::findEntity(Entity* currentEntity, bool isRegularEntity) {
	static AutoCrystal* autoCrystalMod = moduleMgr->getModule<AutoCrystal>();
	autoCrystalMod->entityList.push_back(currentEntity);
	if (!currentEntity->isAlive()) return;
	if (!Target::isValidTarget(currentEntity)) return;
	if (currentEntity->eyePos0.dist(Game.getLocalPlayer()->eyePos0) > autoCrystalMod->targetRange) return;
	autoCrystalMod->targetList.push_back(currentEntity);
}

void AutoCrystal::onTick(GameMode* gm) {
	entityList.clear();
	targetList.clear();
	placeList.clear();
	breakList.clear();
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	BlockSource* region = localPlayer->region;
	if (region == nullptr) return;
	int healthy = Game.getLocalPlayer()->getHealth();
	Game.forEachEntity(AutoCrystal::findEntity);
	if (!targetList.empty()) {
		if (stoph > healthy) return;
		std::sort(targetList.begin(), targetList.end(), AutoCrystal::sortEntityByDist);
		bool check1 = !(stopOnEat && localPlayer->getItemUseDuration() > 0);
		if (autoPlace) generatePlacement(localPlayer, targetList[0]);
		if (autoBreak) getCrystalActorList(localPlayer, targetList[0]);
		if (!check1) return;
		targetname = std::string(targetList[0]->getNameTag()->getText());
		int oldSlot = localPlayer->getSupplies()->selectedHotbarSlot;
		if (switchMode.getSelectedValue() == 1 || switchMode.getSelectedValue() == 2) localPlayer->switchHotbar(getCrystalSlot(localPlayer));
		placeCrystal(gm);
		breakCrystal(gm);
		if (idPredict) breakIdPredictCrystal(gm);
		if (switchMode.getSelectedValue() == 2) {
			localPlayer->switchHotbar(oldSlot);
			gm->player->getSupplies()->selectedHotbarSlot;
		}
	}
	else { targetname = "None"; }
}

void AutoCrystal::onSendPacket(Packet* packet) {
}

void AutoCrystal::onLevelRender() {
	static float duration = 0.f;
	static Vec3 renderPos = Vec3(0.f, 0.f, 0.f);
	DrawUtils::setColor(0.5f, 0.25f, 1.f, 0.75f * duration);
	DrawUtils::drawBox3d(renderPos, renderPos.add(1.f), 1.f, true);
	DrawUtils::setColor(0.5f, 0.25f, 1.f, 0.25f * duration);
	DrawUtils::drawBox3dFilled(renderPos, renderPos.add(1.f), 1.f, false, true);
	int healthy = Game.getLocalPlayer()->getHealth();
	if (!placeList.empty()) {
		if (stoph > healthy) return;
		duration = smoothLerp(1.f, duration, DrawUtils::deltaTime * 10.f);
		Vec3 toPos = placeList[0].placePos.toVec3t();
		if (toPos.dist(renderPos) > 25.f) renderPos = toPos;
		else renderPos = renderPos.add(toPos.sub(renderPos).mul(DrawUtils::deltaTime * 10.f));
		DrawUtils::setColor(0.8f, 0.f, 0.f, 0.2f * duration);
		//DrawUtils::drawBox3dFilled(placeList[0].targetActor->aabb.lower, placeList[0].targetActor->aabb.upper, 1.f, true, true);
		Vec3 lower = placeList[0].targetActor->eyePos0.sub(0.3f, 1.6f, 0.3f);
		Vec3 upper = placeList[0].targetActor->eyePos0.add(0.3f, 0.f, 0.3f);
		DrawUtils::drawBox3dFilled(lower, upper, 1.f, true, true);
	}
	else duration = smoothLerp(0.f, duration, DrawUtils::deltaTime * 7.5f);

}
void AutoCrystal::onPreRender(MinecraftUIRenderContext* renderCtx) {
}