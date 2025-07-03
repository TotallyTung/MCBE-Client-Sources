#include "AutoCrystal.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
/* quick fix prob bad */
int selectedPagsse = 0;
AutoCrystal::AutoCrystal() : Module("AutoCrystal", "Automatically breaks and places Crystal", Category::COMBAT) {
	registerSetting(new PageSetting("Page", "Module Page", { "General", "Range", "Speed", "Inventory", "Render", "Misc" }, & selectedPagsse));
	this->modulePagePtr = &selectedPagsse;
	registerSetting(new BoolSetting("Place", "Place End Crystals at Target", &place, true, 0));
	registerSetting(new BoolSetting("Break", "Explode End Crystals at Target", &explode, true, 0));
	registerSetting(new BoolSetting("MultiTask", "Multitasks like eating and crystalling", &multiTask, true, 0));
	registerSetting(new BoolSetting("Safety", "Prioritizes safety over damage", &safety, true, 0));
	registerSetting(new BoolSetting("Java", "For java servers", &java, false, 0));

	registerSetting(new SliderSetting<int>("PlaceDist", "Range for placing crystals", &placeRange, 5, 1, 12, 1));
	registerSetting(new SliderSetting<int>("BreakDist", "Range for breaking crystals", &breakRange, 5, 1, 12, 1));
	registerSetting(new SliderSetting<int>("TargetDist", "Range for targeting entities", &targetRange, 10, 1, 20, 1));
	registerSetting(new SliderSetting<float>("Proximity", "Proximity for crystal placement", &proximity, 6.f, 1.f, 12.f, 1));
	registerSetting(new SliderSetting<float>("EnemyDmg", "Minimum damage to enemy", &enemyDamage, 8.f, 0.f, 36.f, 1));
	registerSetting(new SliderSetting<float>("SelfDmg", "Maximum damage to self", &localDamage, 4.f, 0.f, 36.f, 1));

	registerSetting(new SliderSetting<int>("PlaceSpeed", "Speed of placing crystals", &placeSpeed, 10, 0, 20, 2));
	registerSetting(new SliderSetting<int>("BreakSpeed", "Speed of breaking crystals", &breakSpeed, 10, 0, 20, 2));
	registerSetting(new SliderSetting<int>("BoostSpeed", "Speed of ID prediction", &predictSpeed, 10, 0, 20, 2));
	registerSetting(new BoolSetting("Boost", "Predict crystal runtime ID for faster actions", &predict, false, 2));
	registerSetting(new SliderSetting<int>("Packets", "Number of packets for prediction", &predictPacket, 5, 1, 10, 2));
	registerSetting(new SliderSetting<float>("BoostDamage", "Minimum damage for boosting", &boostDmg, 10.f, 0.f, 20.f, 2));

	registerSetting(new SliderSetting<int>("WasteAmount", "Number of crystals to place", &wasteAmount, 3, 1, 10, 3));
	registerSetting(new BoolSetting("Rotate", "Rotate to placement locations", &rotate, true, 3));
	registerSetting(new BoolSetting("Swap", "Swap to end crystal", &swap, true, 3));
	registerSetting(new BoolSetting("SwitchBack", "Switch back to previous slot", &switchBack, true, 3));

	registerSetting(new EnumSetting("Render", "Rendering mode for placements", { "Off", "Box", "Flat" }, &renderType, 0, 4));
	registerSetting(new ColorSetting("Color", "Render color", &renderColor, { 255, 0, 0 }, 4));
	registerSetting(new BoolSetting("RenderDamage", "Display damage dealt during render", &dmgText, true, 4));

	registerSetting(new BoolSetting("SelfTest", "Enable testing on yourself", &selfTest, false, 5));
}


bool AutoCrystal::sortCrystal(CrystalData c1, CrystalData c2) {
	return c1.targetDamage > c2.targetDamage;
}

bool AutoCrystal::isPlaceValid(const BlockPos& blockPos, Actor* actor) {
	int blockId = Game::clientInstance->getRegion()->getBlock(blockPos)->blockLegacy->blockId;
	if (!(blockId == 7 || blockId == 49)) return false;
	if (Game::getLocalPlayer()->getEyePos().dist(actor->getEyePos()) > placeRange) return false;
	Vec3 blockBasePos(blockPos.x, blockPos.y + 1, blockPos.z);
	Vec3 blockTopPos(blockPos.x + 1, blockPos.y + 2, blockPos.z + 1);
	const AABB blockAABB(blockBasePos.toFloat(), blockTopPos.toFloat());
	for (Actor* entity : entityList) {
		if (entity->getActorTypeComponent()->id == 71) continue;
		AABB entityAABB = entity->aabbShape->aabb;
		if (entity->getActorTypeComponent()->id == 319 || entity == Game::getLocalPlayer()) {
			Vec3 pos = entity->stateVector->pos;
			entityAABB.lower = Vec3(pos.x - 0.3f, pos.y - 1.6f, pos.z - 0.3f);
			entityAABB.upper = Vec3(pos.x + 0.3f, pos.y, pos.z + 0.3f);

			if (!java) {
				entityAABB.lower = Vec3(entityAABB.lower.x - 0.1f, entityAABB.lower.y - 0.1f, entityAABB.lower.z - 0.1f);
				entityAABB.upper = Vec3(entityAABB.upper.x + 0.1f, entityAABB.upper.y + 0.1f, entityAABB.upper.z + 0.1f);
			}
		}
		if (entityAABB.intersects(blockAABB)) return false;
	}
	return true;
}


void AutoCrystal::generatePlacement(Actor* actor) {
	const int radius = (int)proximity;
	for (int x = -radius; x <= radius; x++) {
		for (int y = -5; y <= 3; y++) {
			for (int z = -radius; z <= radius; z++) {
				const Vec3<float>& targetPos = actor->getHumanPos();
				const BlockPos& blockPos = BlockPos((int)targetPos.x + x, (int)targetPos.y + y, (int)targetPos.z + z);
				if (isPlaceValid(blockPos, actor)) {
					const CrystalPlace placement(actor, blockPos);
					if (placement.targetDamage >= enemyDamage) {
						placeList.emplace_back(placement);
					}
				}
			}
		}
	}
	std::sort(placeList.begin(), placeList.end(), sortCrystal);
}

void AutoCrystal::getCrystals(Actor* actor) {
	for (Actor* entity : entityList) {
		if (entity == nullptr) continue;
		if (entity->getActorTypeComponent()->id != 71) continue;
		if (entity->getEyePos().dist(Game::getLocalPlayer()->getEyePos()) > breakRange) continue;
		const CrystalBreak breakment(actor, entity); // breakment?? :swej:
		if (breakment.targetDamage >= enemyDamage) {
			if (safety && breakment.localDamage <= localDamage) {
				highestId = entity->getRuntimeIDComponent()->runtimeId.id;
				breakList.emplace_back(breakment);
			}
			else if (!safety) {
				highestId = entity->getRuntimeIDComponent()->runtimeId.id;
				breakList.emplace_back(breakment);
			}
		}
	}
	if (!breakList.empty()) std::sort(breakList.begin(), breakList.end(), sortCrystal);
}

void AutoCrystal::placeCrystal(GameMode* gm) {
	if (Game::getLocalPlayer() == nullptr) return;
	if (placeList.empty()) return;
	if (InventoryUtils::getSelectedItemId() != 758) return;
	int placed = 0;
	if (iPlaceDelay >= 20 - placeSpeed) {
		for (const CrystalPlace& place : placeList) {
			gm->buildBlock(place.blockPos, 0, false);
			if (++placed >= wasteAmount) break;
		}
		iPlaceDelay = 0;
	}
	else iPlaceDelay++;
}

void AutoCrystal::breakCrystal(GameMode* gm) {
	if (Game::getLocalPlayer() == nullptr) return;
	if (!breakList.empty()) {
		if (breakList[0].crystal != nullptr) {
			if (iBreakDelay >= 20 - breakSpeed) {
				highestId = breakList[0].crystal->getRuntimeIDComponent()->runtimeId.id;
				gm->attack(breakList[0].crystal);
				iBreakDelay = 0;
			}
			else iBreakDelay++;
		}
		if (breakList[0].targetDamage < boostDmg) return;
	}
	if (!placeList.empty() && predict) {
		const int realPacket = predictPacket * 5;
		if (iBoostDelay >= 20 - predictSpeed) {
			shouldChange = true;
			for (int i = 0; i < realPacket; i++) {
				gm->attack(placeList[0].actor);
				highestId++;
			}
			highestId -= realPacket;
			shouldChange = false;
			iBoostDelay = 0;
		}
		else iBoostDelay++;
	}
}

void AutoCrystal::onNormalTick(LocalPlayer* localPlayer) {
	targetList.clear();
	entityList.clear();
	placeList.clear();
	breakList.clear();
	if (!multiTask && Game::getLocalPlayer()->getItemUseDuration() > 0) return;
	for (Actor* actor : Game::getLocalPlayer()->getlevel()->getRuntimeActorList()) {
		if (!actor) continue;
		entityList.push_back(actor);
		if (!TargetUtil::isTargetValid(actor)) continue;
		if (actor->getEyePos().dist(Game::getLocalPlayer()->getEyePos()) > targetRange) continue;
		targetList.push_back(actor);
	}
	if (selfTest) targetList.push_back(Game::getLocalPlayer());
	if (targetList.empty()) return;
	std::sort(targetList.begin(), targetList.end(), TargetUtil::sortByDist);
	if (place) generatePlacement(targetList[0]);
	if (explode) getCrystals(targetList[0]);
	const int oldSlot = InventoryUtils::getSelectedSlot();
	if (swap) InventoryUtils::SwitchTo(InventoryUtils::getItem(720));
	if (place) placeCrystal(Game::getLocalPlayer()->gameMode);
	if (explode) breakCrystal(Game::getLocalPlayer()->gameMode);
	if (switchBack) InventoryUtils::SwitchTo(oldSlot);
}

void AutoCrystal::onSendPacket(Packet* packet) {
	if (Game::getLocalPlayer() == nullptr) return;
	if (rotate && !placeList.empty()) {
		const Vec2<float>& angle = Game::getLocalPlayer()->getEyePos().CalcAngle(placeList[0].blockPos.toFloat());
		if (packet->getId() == PacketID::PlayerAuthInput) {
			PlayerAuthInputPacket* authPkt = (PlayerAuthInputPacket*)packet;
			authPkt->rotation = angle; 
			authPkt->headYaw = angle.y;
		}
		if (!predict || !shouldChange) return;
		if (packet->getId() == PacketID::InventoryTransaction) {
			InventoryTransactionPacket* invPkt = (InventoryTransactionPacket*)packet;
			ComplexInventoryTransaction* invComplex = invPkt->transaction.get();
			if (invComplex->type == ComplexInventoryTransaction::Type::ItemUseOnEntityTransaction) *(int*)((uintptr_t)(invComplex)+0x68) = highestId;
		}
	}
}
