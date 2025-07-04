#include "../../Utils/GuiUtils.h"
#include "AutoCrystalRewrite.h"//No iq
float lerpSpeed = 0.3f;
std::string targetname = "None";
int stoph = 6;
AutoCrystal2::AutoCrystal2() : IModule(0x0, Category::COMBAT, "Automatically places and breaks end crystals.") {
	registerFloatSetting("Target Range", &targetRange, targetRange, 5.f, 20.f);
	// Place
	registerBoolSetting("Auto Place", &autoPlace, autoPlace);
	registerFloatSetting("Place Range", &placeRange, placeRange, 3.f, 10.f);
	registerFloatSetting("Max Crystal Range", &crystalRange, crystalRange, 1.f, 6.f);
	registerFloatSetting("Min Place Dame", &minPlaceDame, minPlaceDame, 1.f, 20.f);
	registerFloatSetting("Max Place Dame", &maxPlaceDame, maxPlaceDame, 1.f, 20.f);
	registerIntSetting("MultiPlace", &multiPlace, multiPlace, 1, 5);
	registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 20);
	// Break
	registerBoolSetting("Auto Break", &autoBreak, autoBreak);
	registerFloatSetting("Break Range", &breakRange, breakRange, 3.f, 10.f);
	registerFloatSetting("Min Break Dame", &minBreakDame, minBreakDame, 1.f, 20.f);
	registerFloatSetting("Max Break Dame", &maxBreakDame, maxBreakDame, 1.f, 20.f);
	registerIntSetting("Break Delay", &breakDelay, breakDelay, 0, 20);
	registerBoolSetting("ID Predict", &idPredict, idPredict);
	registerIntSetting("Packets", &packets, packets, 1, 30);
	registerIntSetting("Send Delay", &sendDelay, sendDelay, 0, 20);
	// Switch
	switchMode.addEntry("None", 0);
	switchMode.addEntry("Regular", 1);
	switchMode.addEntry("NukkitSpoof", 2);
	registerEnumSetting("Switch", &switchMode, 0);
	registerBoolSetting("Stop On Eat", &stopOnEat, stopOnEat);
	registerIntSetting("Stop Health", &stoph, stoph, 5, 20);
	registerFloatSetting("Slide Speed", &lerpSpeed, lerpSpeed, 0.f, 1.f);
	registerFloatSetting("FillAlpha", &alpha, alpha, 0.f, 1.f);
	registerFloatSetting("R", &R, R, 0.f, 1.f);
	registerFloatSetting("B", &B, B, 0.f, 1.f);
	registerFloatSetting("G", &G, G, 0.f, 1.f);
	registerFloatSetting("BoxAlpha", &alpha2, alpha2, 0.f, 1.f);
};

const char* AutoCrystal2::getModuleName() {
	return ("AutoCrystal");

}

const char* AutoCrystal2::getModName() {
	return targetname.c_str();
}

void AutoCrystal2::onEnable() {
}
void AutoCrystal2::onDisable() {
}

bool AutoCrystal2::sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2) {
	return a1.TgDameTake > a2.TgDameTake;
}

bool AutoCrystal2::sortEntityByDist(C_Entity* a1, C_Entity* a2) {
	vec3_t localPlayerPos = g_Data.getLocalPlayer()->getHumanPos();
	return a1->getHumanPos().dist(localPlayerPos) < a2->getHumanPos().dist(localPlayerPos);
}

bool AutoCrystal2::isHoldingCrystal() {
	return (g_Data.getLocalPlayer()->getSelectedItemId() == 637);
}

bool AutoCrystal2::canPlaceCrystal(C_BlockSource* blockSource, const vec3_ti& placePos, C_Entity* tg) {
	C_Block* block = blockSource->getBlock(placePos);
	C_Block* upper1 = blockSource->getBlock(placePos.add(0, 1, 0));
	C_Block* upper2 = blockSource->getBlock(placePos.add(0, 2, 0));
	if (block == nullptr || upper1 == nullptr || upper2 == nullptr) return false;
	if (!(block->blockLegacy->blockId == 49 || block->blockLegacy->blockId == 7)) return false;
	if (upper1->blockLegacy->blockId != 0) return false;
	if (upper2->blockLegacy->blockId != 0) return false;
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer->getHumanPos().dist(placePos.toVector3().add(0.5f)) > placeRange) return false;

	vec3_t _Pos = placePos.toVector3();
	_Pos.y += 1.f;
	if (tg->aabb.intersects(AABB(_Pos, _Pos.add(1.f)))) return false;
	for (C_Entity* ent : entityList) {
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

int AutoCrystal2::getCrystalSlot(C_LocalPlayer* localPlayer) {
	C_PlayerInventoryProxy* supplies = localPlayer->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 9; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->isValid()) {
			if (stack->getItem()->itemId == 637) {
				return i;
			}
		}
	}
	return -1;
}

void AutoCrystal2::generatePlacement(C_LocalPlayer* localPlayer, C_Entity* target) {
	C_BlockSource* region = localPlayer->region;
	int radius = (int)(crystalRange);
	for (int x = -radius; x <= radius; x++) {
		for (int y = -3; y <= 3; y++) {
			for (int z = -radius; z <= radius; z++) {
				vec3_t targetPos = target->getHumanPos();
				vec3_ti blockPos((int)targetPos.x + x, (int)targetPos.y + y, (int)targetPos.z + z);
				if (canPlaceCrystal(region, blockPos, target)) {
					CrystalPlacements placement(blockPos, localPlayer, target);
					if (placement.LpDameTake < maxPlaceDame && placement.TgDameTake >= minPlaceDame) placeList.push_back(placement);
				}
			}
		}
	}
	std::sort(placeList.begin(), placeList.end(), AutoCrystal2::sortCrystalByTargetDame);
}

void AutoCrystal2::placeCrystal(C_GameMode* gm) {
	if (placeList.empty()) return;
	if (!isHoldingCrystal()) return;
	if (placeDelayTick >= placeDelay) {
		int placed = 0;
		for (CrystalPlacements placement : placeList) {
			gm->buildBlock(&placement.placePos, random(0, 5));
			placed++;
			if (placed >= multiPlace) break;
		}
		placeDelayTick = 0;
	}
	else {
		placeDelayTick++;
	}
}

void AutoCrystal2::getCrystalActorList(C_LocalPlayer* localPlayer, C_Entity* target) {
	for (C_Entity* ent : entityList) {
		if (ent->getEntityTypeId() != 71) continue;
		if (*(int*)ent->getUniqueId() > highestID) highestID = *(int*)ent->getUniqueId();
		if (ent->eyePos0.dist(localPlayer->getHumanPos()) > breakRange) continue;
		CrystalBreaker cBreaker(ent, localPlayer, target);
		if (cBreaker.LpDameTake < maxBreakDame && cBreaker.TgDameTake >= minBreakDame) breakList.push_back(cBreaker);
	}
	std::sort(breakList.begin(), breakList.end(), AutoCrystal2::sortCrystalByTargetDame);
}

void AutoCrystal2::breakCrystal(C_GameMode* gm) {
	if (!autoBreak) return;
	if (!breakList.empty()) {
		if (breakDelayTick >= breakDelay) {
			highestID = *(int*)breakList[0].crystalActor->getUniqueId();
			gm->attack(breakList[0].crystalActor);
			gm->attack(breakList[0].crystalActor);
			breakDelayTick = 0;
		}
		else {
			breakDelayTick++;
		}
	}
}

void AutoCrystal2::breakIdPredictCrystal(C_GameMode* gm) {
	if (!autoBreak) return;
	if (placeList.empty()) return;
	if (sendDelayTick >= sendDelay) {
		shouldChangeID = true;
		for (int i = 0; i < packets; i++) {
			gm->attack(placeList[0].targetActor);
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
void AutoCrystal2::findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static AutoCrystal2* autoCrystalMod = moduleMgr->getModule<AutoCrystal2>();
	autoCrystalMod->entityList.push_back(currentEntity);
	if (!currentEntity->isAlive()) return;
	if (!TargetUtil::isValidTarget(currentEntity)) return;
	if (currentEntity->eyePos0.dist(g_Data.getLocalPlayer()->eyePos0) > autoCrystalMod->targetRange) return;
	autoCrystalMod->targetList.push_back(currentEntity);
}

void AutoCrystal2::onTick(C_GameMode* gm) {
	entityList.clear();
	targetList.clear();
	placeList.clear();
	breakList.clear();
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_BlockSource* region = localPlayer->region;
	if (region == nullptr) return;
	int healthy = g_Data.getLocalPlayer()->getHealth();
	g_Data.forEachEntity(AutoCrystal2::findEntity);
	if (!targetList.empty()) {
		if (stoph > healthy) return;
		std::sort(targetList.begin(), targetList.end(), AutoCrystal2::sortEntityByDist);
		bool check1 = !(stopOnEat && localPlayer->getC_ItemUseDuration() > 0);
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
	else {
		targetname = " None";
	}
}
float roundoff(float value, unsigned char prec) {
	float pow_10 = pow(10.0f, (float)prec);
	return round(value * pow_10) / pow_10;
}

std::string chopOffDigits(std::string STUFF, int digits) {
	bool afterDecimalPt = false;
	int dp = 0;
	std::string toOutput;

	for (int i = 0; i < STUFF.length(); ++i) {
		if (STUFF.at(i) == '.') {
			afterDecimalPt = true;
		}

		toOutput.append(std::string(1, STUFF.at(i)));

		if (afterDecimalPt) {
			dp++;

			if (dp > digits)
				break;
		}
	}

	return toOutput;
}
void AutoCrystal2::onSendPacket(C_Packet* packet) {
}

void AutoCrystal2::onLevelRender() {
	static float duration = 0.f;
	static vec3_t renderPos = vec3_t(0.f, 0.f, 0.f);
	DrawUtils::setColor(R, G, B, alpha * duration);
	DrawUtils::drawBox3d(renderPos, renderPos.add(1.f));
	DrawUtils::setColor(R, G, B, alpha * duration);
	//DrawUtils::drawBox3dFilled(renderPos, renderPos.add(1.f), 1.f, true, true);
	int healthy = g_Data.getLocalPlayer()->getHealth();
	if (!placeList.empty()) {
		if (stoph > healthy) return;
		duration = smoothLerp(1.f, duration, lerpSpeed);
		vec3_t toPos = placeList[0].placePos.toVector3();
		if (toPos.dist(renderPos) > 25.f)
			renderPos = toPos;
		else
			renderPos = renderPos.add(toPos.sub(renderPos).mul(lerpSpeed));
		DrawUtils::setColor(0.8f, 0.f, 0.f, 0.2f * duration);
		// DrawUtils::drawBox3dFilled(placeList[0].targetActor->aabb.lower, placeList[0].targetActor->aabb.upper, 1.f, true, true);
		vec3_t lower = placeList[0].targetActor->eyePos0.sub(0.3f, 1.6f, 0.3f);
		vec3_t upper = placeList[0].targetActor->eyePos0.add(0.3f, 0.f, 0.3f);
		//DrawUtils::drawBox3dFilled(lower, upper, 1.f, true, true);
	}
	else
		duration = smoothLerp(0.f, duration, lerpSpeed);
}