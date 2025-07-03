#include "AutoCrystal.h"
#include "../../Utils/ImGuiUtils.h"
#include "../ModuleManager.h"
std::string ExplosionDamage;
std::string targetname = "None";
int stoph = 6;
float smooth = 10;
bool baseplace;
bool fullbaseplace;
int mckey = false;
bool ShowDamage = true;
inline SettingEnum caModeIDPredict;
inline SettingEnum RenderType;
bool afterimage = true;
AutoCrystal::AutoCrystal() : IModule(0x0, Category::COMBAT, "Like CrystalAura lol") {
	registerBoolSetting("Priority Mine", &pm, true);
	caModeIDPredict = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("idPredict", 1));
	registerEnumSetting("Mode", &caModeIDPredict, 1);
	registerKeybindSetting("ModeChangeKey", &mckey, mckey);
	registerBoolSetting("BasePlace", &baseplace, true);
	registerBoolSetting("FullBasePlace", &fullbaseplace, true);

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
	registerIntSetting("Packets", &packets, packets, 1, 30);
	registerIntSetting("Send Delay", &sendDelay, sendDelay, 0, 20);
	//Switch
	switchMode.addEntry("None", 0);
	switchMode.addEntry("Normal", 1);
	RenderType = SettingEnum(this)
		.addEntry("None", 0)
		.addEntry("Face", 1)
		.addEntry("Outline", 2)
		.addEntry("Filled", 3)
		.addEntry("Both", 4);
	registerEnumSetting("RenderType", &RenderType, 1);
	registerBoolSetting("After Image", &afterimage, true);
	registerBoolSetting("Render Damage", &ShowDamage, ShowDamage);
	registerEnumSetting("Switch", &switchMode, 0);
	registerIntSetting("Stop Health", &stoph, stoph, 0, 36);
	registerFloatSetting("Smooth", &smooth, smooth, 1.f, 50.f);
};
const char* AutoCrystal::getModuleName() {
	return "AutoCrystal";
}
void AutoCrystal::onKeyUpdate(int keyb, bool isDown) {
	if (!Game.canUseMoveKeys()) return;
	if (!isDown) return;
	if (keyb == this->getKeybind()) {
		this->toggle();
	}
	if (keyb != mckey && !this->isEnabled()) return;
	if (Game.getLocalPlayer() == nullptr) return;
	if (caModeIDPredict.getSelectedValue() == 1 && keyb == mckey) {
		caModeIDPredict.selected = 0;
		Notifications::addNotifBox("Changed CA Mode! {Slow}", 5.f);
	}
	else if (caModeIDPredict.getSelectedValue() == 0 && keyb == mckey) {
		caModeIDPredict.selected = 1;
		Notifications::addNotifBox("Changed CA Mode! {Fast}", 5.f);
	}
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
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->isValid()) {
			if (stack->getItem()->itemId == 637) {
				return n;
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
					if (placement.LpDameTake < maxPlaceDame && placement.TgDameTake >= minPlaceDame) {
						ExplosionDamage = std::to_string((int)placement.LpDameTake + (int)placement.TgDameTake / 2);
						placeList.push_back(placement);
					};
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
	if (currentEntity == nullptr) return;
	static AutoCrystal* autoCrystalMod = moduleMgr->getModule<AutoCrystal>();
	autoCrystalMod->entityList.push_back(currentEntity);
	if (!currentEntity->isAlive()) return;
	if (!Target::isValidTarget(currentEntity)) return;
	if (currentEntity->eyePos0.dist(Game.getLocalPlayer()->eyePos0) > autoCrystalMod->targetRange) return;
	autoCrystalMod->targetList.push_back(currentEntity);
}
bool canplaBasePlace(Vec3i blockPos) {
	if (Game.getLocalPlayer()->getPos()->dist(blockPos.toVec3t()) >= 7) {
		return false;
	}
	else return true;
}
bool tryAutoTrapBasePlace(Vec3i blkPlacement) {
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
float waitsec = 0.f;
void AutoCrystal::onTick(GameMode* gm) {
	entityList.clear();
	targetList.clear();
	placeList.clear();
	breakList.clear();
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	BlockSource* region = localPlayer->region;
	if (localPlayer == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	if (region == nullptr) return;
	if (pm) {
		if (moduleMgr->getModule<PacketMine>()->blockPos != Vec3(0, 0, 0)) {
			waitsec = 0.5f;
		}
		if (waitsec >= 0.1f) {
			waitsec -= 0.1f;
			return;
		}
	}
	int healthy = Game.getLocalPlayer()->getHealth();
	Game.forEachEntity(AutoCrystal::findEntity);
	if (!targetList.empty()) {
		if (stoph > healthy) return;
		std::sort(targetList.begin(), targetList.end(), AutoCrystal::sortEntityByDist);
		bool check1 = !(localPlayer->getItemUseDuration() > 0);
		if (moduleMgr->getModule<Desync>()->isEnabled() && (Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && (Game.isRightClickDown() || moduleMgr->getModule<PacketGapple>()->isEnabled() && Game.getLocalPlayer()->getHealth() < moduleMgr->getModule<PacketGapple>()->health)))
			return;
		for (Entity* tgt : targetList) {
			if (autoPlace) generatePlacement(localPlayer, tgt);
			if (autoBreak) getCrystalActorList(localPlayer, tgt);
			if (!check1) return;
			targetname = std::string(tgt->getNameTag()->getText());
			int oldSlot = localPlayer->getSupplies()->selectedHotbarSlot;
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			Inventory* inv = supplies->inventory;
			int slotab = 0;
			if (baseplace) {
				if (placeList.empty()) {
					for (Entity* a : targetList) {
						Vec3i targetPosi = a->getPos()->floor();
						for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
							for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
								slotab = supplies->selectedHotbarSlot;
								for (int n = 0; n < 36; n++) {
									ItemStack* stack = inv->getItemStack(n);
									if (stack->item != nullptr) {
										if (stack->getItem()->itemId == 49) {
											supplies->selectedHotbarSlot = n;
											break;
										}
									}
								};
								if (canplaBasePlace(Vec3i(x, (int)targetPosi.y - 1.6f, z))) {
									tryAutoTrapBasePlace(Vec3i(x, (int)targetPosi.y - 1.6f, z));
								}
								supplies->selectedHotbarSlot = slotab;
							}
						}
					}
				}
				if (fullbaseplace) {
					for (Entity* a : targetList) {
						Vec3i targetPosi = a->getPos()->floor();
						for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
							for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
								slotab = supplies->selectedHotbarSlot;
								for (int n = 0; n < 36; n++) {
									ItemStack* stack = inv->getItemStack(n);
									if (stack->item != nullptr) {
										if (stack->getItem()->itemId == 49) {
											supplies->selectedHotbarSlot = n;
											break;
										}
									}
								};
								if (canplaBasePlace(Vec3i(x, (int)targetPosi.y - 1.6f, z))) {
									tryAutoTrapBasePlace(Vec3i(x, (int)targetPosi.y - 1.6f, z));
								}
								supplies->selectedHotbarSlot = slotab;
							}
						}
					}
				};
			}
			if (switchMode.getSelectedValue() == 1 || moduleMgr->getModule<Silent>()->isEnabled()) localPlayer->switchHotbar(getCrystalSlot(localPlayer));
			placeCrystal(gm);
			breakCrystal(gm);
			if (caModeIDPredict.getSelectedValue() == 1) breakIdPredictCrystal(gm);
			if (moduleMgr->getModule<Silent>()->isEnabled()) {
				localPlayer->switchHotbar(oldSlot);
				gm->player->getSupplies()->selectedHotbarSlot;
			}
		}
	}
	else { targetname = "None"; }
}
Entity* AutoCrystal::getTargetEntity() {
	return moduleMgr->getModule<AutoCrystal>()->targetList[0];
}
void AutoCrystal::onLevelRender() {
	static float duration = 0.f;
	static Vec3 renderPos = Vec3(0.f, 0.f, 0.f);
	int healthy = Game.getLocalPlayer()->getHealth();
	if (!targetList.empty()) {
		if (baseplace) {
			if (placeList.empty()) {
				for (Entity* a : targetList) {
					Vec3i targetPosi = a->getPos()->floor();
					for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
						for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
							MC_Color ar = ColorUtil::astolfoRainbow(0);
							DrawUtils::setColor(ar.r, ar.g, ar.b, 0.25f * duration);
							DrawUtils::drawBox3dFilled(Vec3(x, targetPosi.y - 2, z), Vec3(x, targetPosi.y - 2, z).add(1.f), 1.f, false, true);
						}
					}
				}
			}
			if (fullbaseplace) {
				for (Entity* a : targetList) {
					Vec3i targetPosi = a->getPos()->floor();
					for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++) {
						for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++) {
							MC_Color ar = ColorUtil::astolfoRainbow(0);
							DrawUtils::setColor(ar.r, ar.g, ar.b, 0.25f * duration);
							DrawUtils::drawBox3dFilled(Vec3(x, targetPosi.y - 2, z), Vec3(x, targetPosi.y - 2, z).add(1.f), 1.f, false, true);
						}
					}
				}
			};
		}
	}
	else duration = smoothLerp(0.f, duration, DrawUtils::deltaTime * smooth - 1.5f);
	if (!placeList.empty()) {
		MC_Color C = moduleMgr->getModule<Colors>()->getColorV2(1);
		if (RenderType.getSelectedValue() == 1) {
			DrawUtils::setColor(C.r, C.g, C.b, 0.3f * duration);
			DrawUtils::drawBox3dFilled(renderPos.add(0.f, 0.98f, 0.f), renderPos.add(1.f, 1.f, 1.f), 1.f, true);
		};
		if (RenderType.getSelectedValue() == 2 || RenderType.getSelectedValue() == 4) {
			DrawUtils::setColor(C.r, C.g, C.b, 0.5f * duration);
			DrawUtils::drawBox3d(renderPos, renderPos.add(1.f), 1.f, true);
		};
		if (RenderType.getSelectedValue() == 3) {
			DrawUtils::setColor(C.r, C.g, C.b, 0.12f * duration);
			DrawUtils::drawBox3dFilled(renderPos, renderPos.add(1.f), 1.f, false, true);
		};
		if (RenderType.getSelectedValue() == 4) {
			DrawUtils::setColor(C.r, C.g, C.b, 0.5f * duration);
			DrawUtils::drawBox3d(renderPos, renderPos.add(1.f), 1.f, true);
			DrawUtils::setColor(C.r, C.g, C.b, 0.12f * duration);
			DrawUtils::drawBox3dFilled(renderPos, renderPos.add(1.f), 1.f, false, true);
		};
		duration = smoothLerp(1.f, duration, DrawUtils::deltaTime * 10.f);
		Vec3 toPos = placeList[0].placePos.toVec3t();
		if (toPos.dist(renderPos) > 25.f) renderPos = toPos;
		else renderPos = renderPos.add(toPos.sub(renderPos).mul(DrawUtils::deltaTime * smooth));
	}
	else duration = smoothLerp(0.f, duration, DrawUtils::deltaTime * smooth - 1.5f);
}
void AutoCrystal::onPreRender(MinecraftUIRenderContext* renderCtx) {
	int healthy = Game.getLocalPlayer()->getHealth();
	static Vec3 renderPos = Vec3(0.f, 0.f, 0.f);
	if (!placeList.empty()) {
		Vec3 toPos = placeList[0].placePos.toVec3t();
		if (toPos.dist(renderPos) > 25.f) renderPos = toPos;
		else renderPos = renderPos.add(toPos.sub(renderPos).mul(DrawUtils::deltaTime * smooth));
		Vec2 textPos = DrawUtils::worldToScreen(renderPos.add(0.5f, 0.5f, 0.5f));
		textPos.x -= DrawUtils::getTextWidth(&ExplosionDamage, 0.8f) / 2.f;
		textPos.y -= DrawUtils::getFontHeight(0.8f) / 2.f;
		if ((ShowDamage) && (RenderType.getSelectedValue() != 0)) DrawUtils::drawText(textPos, &ExplosionDamage, ColorUtil::astolfoRainbow(0), 0.8f);
	};
}
void AutoCrystal::onJoiningServer() {
	entityList.clear();
	targetList.clear();
	placeList.clear();
	breakList.clear();
}