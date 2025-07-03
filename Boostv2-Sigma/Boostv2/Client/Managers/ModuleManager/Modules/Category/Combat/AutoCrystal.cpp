#include "AutoCrystal.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
UIColor lineColor;
int ppackets = 5;
int bpackets = 5;
int logic = 0;
bool breakType;
AutoCrystal::AutoCrystal() : Module("AutoCrystal", "automatically breaks and places crystal", Category::COMBAT) {
		registerSetting(new PageSetting("Page", "Module Page", {
			"Place", "Break", "Target", "Damage", "Switch", "Predict", "Render", "Misc"
			}, &pageset));

		this->modulePagePtr = &pageset; 
	// Page 0: Place
	registerSetting(new BoolSetting("Place", "place end crystals at target", &place, true, 0));
	registerSetting(new SliderSetting<int>("PlaceDist", "range for placing crystals", &placeRange, 5, 1, 12, 0));
	registerSetting(new SliderSetting<int>("PlaceSpeed", "speed of placing crystals", &placeSpeed, 10, 0, 100, 0));
	registerSetting(new SliderSetting<int>("PlacePackets", "number of packets for prediction", &ppackets, 5, 1, 10, 0));
	registerSetting(new SliderSetting<float>("Proximity", "proximity for crystal placement", &proximity, 6.f, 1.f, 12.f, 0));
	registerSetting(new BoolSetting("Rotate", "rotate to placement locations", &rotate, true, 0));
	registerSetting(new SliderSetting<int>("WasteAmount", "number of crystals to place", &wasteAmount, 3, 1, 10, 0));
	registerSetting(new EnumSetting("Logic", "Logic", { "Place/Break", "Break/Place" }, &logic, 0, 0));

	// Page 1: Break
	registerSetting(new BoolSetting("Break", "explode end crystals at target", &explode, true, 1));
	registerSetting(new BoolSetting("All", "Break all Crystals", &breakType, true, 1));
	registerSetting(new SliderSetting<int>("BreakDist", "range for breaking crystals", &breakRange, 5, 1, 12, 1));
	registerSetting(new SliderSetting<int>("BreakSpeed", "speed of breaking crystals", &breakSpeed, 10, 0, 110, 1));
	registerSetting(new SliderSetting<int>("BreakPackets", "number of packets for prediction", &bpackets, 5, 1, 10, 1));

	// Page 2: Target
	registerSetting(new SliderSetting<int>("TargetDist", "range for targeting entities", &targetRange, 10, 1, 20, 2));
	registerSetting(new BoolSetting("selftest", "enable testing on yourself", &selfTest, false, 2));

	// Page 3: Damage
	registerSetting(new SliderSetting<float>("Enemydmg", "minimum damage to enemy", &enemyDamage, 8.f, 0.f, 36.f, 3));
	registerSetting(new SliderSetting<float>("Selfdmg", "maximum damage to self", &localDamage, 4.f, 0.f, 50.f, 3));

	// Page 4: Switch
	registerSetting(new BoolSetting("Swap", "swap to end crystal", &swap, true, 4));
	registerSetting(new BoolSetting("Switchback", "switch back to previous slot", &switchBack, true, 4));

	// Page 5: Predict
	registerSetting(new BoolSetting("Boost", "predict crystal runtime id for faster actions", &predict, false, 5));
	registerSetting(new SliderSetting<int>("BoostSpeed", "speed of id prediction", &predictSpeed, 10, 0, 100, 5));
	registerSetting(new SliderSetting<int>("PredictPacket", "number of packets for prediction", &predictPacket, 5, 1, 20, 5));
	registerSetting(new SliderSetting<float>("BoostDamage", "minimum damage for boosting", &boostDmg, 10.f, 0.f, 20.f, 5));

	// Page 6: Render
	registerSetting(new EnumSetting("Render", "rendering mode for placements", { "None", "Box", "Flat" }, &renderType, 0, 6));
	registerSetting(new ColorSetting("Color", "render color", &renderColor, { 255, 255, 255 }, 6));
	registerSetting(new ColorSetting("Line Color", "render  line color", &lineColor, { 255, 255, 255 , 85 }, 6));

	// Page 7: Misc
	registerSetting(new BoolSetting("Multitask", "multitasks like eating and crystalling", &multiTask, true, 7));
	registerSetting(new BoolSetting("Java", "for java servers", &java, false, 7));
	registerSetting(new BoolSetting("Safety", "prioritizes safety over damage", &safety, true, 7));

}

bool AutoCrystal::sortCrystal(CrystalData c1, CrystalData c2) {
	return c1.targetDamage > c2.targetDamage;
}
void AutoCrystal::predictEndCrystal(GameMode* gm) {
	if (placeList.empty()) return;
	int realPacket = predictPacket * 5;
	if (iBoostDelay >= std::max(1, 2 - (predictSpeed / 40))) {
		shouldChange = true;
		for (int i = 0; i < realPacket; i++) {
			std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::Text);
			auto* pkt = reinterpret_cast<InteractPacket*>(packet.get());

			pkt->mTargetId = placeList[0].actor->getRuntimeIDComponent()->runtimeId.id;
			pkt->mAction = InteractAction::LEFT_CLICK;
			pkt->mPos = placeList[0].actor->stateVector->pos.sub(Vec3<float>(0.f, 0.2f, 0.f));
			if (!breakList.empty()) gm->attack(breakList[0].crystal);
			Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(pkt);
			highestId++;
		}
		highestId -= realPacket;
		shouldChange = false;
		iBoostDelay = 0;
	}
	else iBoostDelay++;
}
float calculateDamage(const BlockPos& crystalPos, Actor* target) { //this is basic but works
	Vec3<float> crystalPosFloat(static_cast<float>(crystalPos.x) + 0.5f,
		static_cast<float>(crystalPos.y) + 1.0f,
		static_cast<float>(crystalPos.z) + 0.5f);
	Vec3<float> targetPos = target->getHumanPos();
	targetPos.y += 0.5f;
	float distance = crystalPosFloat.dist(targetPos);
	distance = std::max(0.0f, std::min(distance, 12.0f));
	float damage = (12.0f * (1.0f - (distance / 6.0f)));
	return std::max(0.0f, damage);
}

bool AutoCrystal::isPlaceValid(const BlockPos& blockPos, Actor* actor) {
	BlockLegacy* block = Game.clientInstance->getRegion()->getBlock(blockPos)->blockLegacy;
	if (!(block->blockId == 7 || block->blockId == 49))
		return false;
	Vec3<float> blockPosFloat(blockPos.x, blockPos.y, blockPos.z);
	Vec3<float> pos = Vec3<float>(blockPos.x + 0.5f, blockPos.y + 1.0f, blockPos.z + 0.5f);
	if (Game.getLocalPlayer()->getEyePos().dist(pos) > placeRange)
		return false;
	if (!(Game.getClientInstance()->getRegion()->getBlock(blockPos)->blockLegacy->blockId == 49 || Game.getClientInstance()->getRegion()->getBlock(blockPos)->blockLegacy->blockId == 7)) return false;
	if (!Game.getClientInstance()->getRegion()->getBlock(blockPos.add2(0, 1, 0))->blockLegacy->blockId == 0) return false;
	if (!java && !Game.getClientInstance()->getRegion()->getBlock(blockPos.add2(0, 2, 0))->blockLegacy->blockId == 0) return false;
	Vec3<float> lower(blockPos.x, blockPos.y, blockPos.z);
	Vec3<float> upper(blockPos.x + 1, blockPos.y + 2, blockPos.z + 1);
	const AABB blockAABB(lower, upper);

	for (Actor* entity : entityList) {
		if (entity->getActorTypeComponent()->id == 71) continue; // Skip crystals
		AABB entityAABB = entity->aabbShape->aabb;
		if (entity->getActorTypeComponent()->id == 319 || entity == Game.getLocalPlayer()) {
			Vec3<float> pos = entity->stateVector->pos;
			entityAABB.lower = pos.sub(Vec3<float>(0.3f, 0.0f, 0.3f));
			entityAABB.upper = pos.add(Vec3<float>(0.3f, 1.8f, 0.3f));
			if (!java) entityAABB = entityAABB.expand(0.1f);
		}

		if (entityAABB.intersects(blockAABB)) return false;
	}

	return true;
}
void AutoCrystal::generatePlacement(Actor* actor) {
	placeList.clear();

	const Vec3 targetPos = actor->getHumanPos();
	const BlockPos center((int)targetPos.x, (int)targetPos.y, (int)targetPos.z);
	int radius = (int)proximity;

	for (int x = -radius; x <= radius; x++) {
		for (int y = -2; y <= 2; y++) {
			for (int z = -radius; z <= radius; z++) {
				BlockPos blockPos = BlockPos(center.x + x, center.y + y, center.z + z);
				Vec3<float> blockPosFloat(blockPos.x + 0.5f, blockPos.y + 1.0f, blockPos.z + 0.5f);
				float dx = (blockPos.x + 0.5f) - targetPos.x;
				float dz = (blockPos.z + 0.5f) - targetPos.z;
				float distance = sqrtf(dx * dx + dz * dz);

				if (distance > placeRange || distance < 1.0f) continue;

				if (isPlaceValid(blockPos, actor)) {
					CrystalPlace placement(actor, blockPos);
					placement.targetDamage = calculateDamage(blockPos, actor);
					placement.localDamage = calculateDamage(blockPos, Game.getLocalPlayer());

					if (placement.targetDamage >= enemyDamage &&
						(!safety || placement.localDamage <= localDamage)) {
						placeList.emplace_back(placement);
					}
				}
			}
		}
	}
	std::sort(placeList.begin(), placeList.end(), [&](const CrystalPlace& a, const CrystalPlace& b) {
		if (a.targetDamage != b.targetDamage)
			return a.targetDamage > b.targetDamage;

		if (a.localDamage != b.localDamage)
			return a.localDamage < b.localDamage;

		float distA = Vec3<float>(a.blockPos.x + 0.5f, a.blockPos.y + 1.0f, a.blockPos.z + 0.5f).dist(targetPos);
		float distB = Vec3<float>(b.blockPos.x + 0.5f, b.blockPos.y + 1.0f, b.blockPos.z + 0.5f).dist(targetPos);
		if (distA != distB)
			return distA < distB;

		Vec3<float> eyePos = Game.getLocalPlayer()->getEyePos();
		float selfDistA = Vec3<float>(a.blockPos.x + 0.5f, a.blockPos.y + 1.0f, a.blockPos.z + 0.5f).dist(eyePos);
		float selfDistB = Vec3<float>(b.blockPos.x + 0.5f, b.blockPos.y + 1.0f, b.blockPos.z + 0.5f).dist(eyePos);
		return selfDistA < selfDistB;
		});

}
void AutoCrystal::getCrystals(Actor* actor) {
	for (Actor* entity : entityList) {
		if (entity == nullptr) continue;
		if (entity->getActorTypeComponent()->id != 71) continue;
		if (entity->getEyePos().dist(Game.getLocalPlayer()->getEyePos()) > breakRange) continue;
		const CrystalBreak breakment(actor, entity); // breakment?? :swej:
		if (breakment.targetDamage >= enemyDamage) {
			if (safety && breakment.localDamage <= localDamage) {
				if(predict)highestId = entity->getRuntimeIDComponent()->runtimeId.id;
				breakList.emplace_back(breakment);
			}
			else if (!safety) {
				if(predict)highestId = entity->getRuntimeIDComponent()->runtimeId.id;
				breakList.emplace_back(breakment);
			}
		}
	}
	if (!breakList.empty()) std::sort(breakList.begin(), breakList.end(), sortCrystal);
}

void AutoCrystal::placeCrystal(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr || placeList.empty()) return;
	if (InventoryUtils::getSelectedItemId() != 758) return;

	int placed = 0;

	if (iPlaceDelay >= std::max(1, 20 - placeSpeed)) {
		for (const CrystalPlace& place : placeList) {
			for (int i = 0; i < ppackets; i++) {
				gm->buildBlock(place.blockPos, Math::randomInt(0,5), false);
			}
			if (++placed >= wasteAmount) break;
		}
		iPlaceDelay = 0;
	}
	else {
		iPlaceDelay++;
	}
}

void AutoCrystal::breakCrystal(GameMode* gm) {
	if (breakList.empty()) return;
	PlayerInventory* plrInv = Game.getLocalPlayer()->getplayerInventory();
	Container* inv = plrInv->container;
	int breakInterval = std::max(1, 4 - (breakSpeed / 25));
	if (iBreakDelay >= breakInterval) {
		if (predict) highestId = breakList[0].crystal->getRuntimeIDComponent()->runtimeId.id;
		if (breakType) {
			for (Actor* crystal : Game.getLocalPlayer()->getlevel()->getRuntimeActorList()) {
				if (crystal == nullptr) continue;
				if (crystal->getActorTypeComponent()->id != 71) continue;
				if (crystal->getEyePos().dist(Game.getLocalPlayer()->getEyePos()) > breakRange) continue;
				gm->attack(crystal);
				break;
			}
		}
		gm->attack(breakList[0].crystal);
		iBoostDelay = 0;
	}
	else iBoostDelay++;
}

bool SpoofSwitcheze(int slot) {
	auto player = Game.getLocalPlayer();
	if (!player) return false;
	auto itemStack = player->getplayerInventory()->container->getItem(slot);
	if (!itemStack)
	{
		return false;
	}


	auto mep = MinecraftPackets::createPacket(PacketID::MobEquipment);
	auto* pkt = reinterpret_cast<MobEquipmentPacket*>(mep.get());
	//auto* pkt = reinterpret_cast<MobEquipmentPacket*>(mep.get());

	//pkt->mRuntimeId = player->getComponent<RuntimeIDComponent>()->runtimeID;
	pkt->mSlot = slot;
	pkt->mSelectedSlot = slot;
	pkt->mContainerId = 0;
	pkt->mSlotByte = slot;
	pkt->mSelectedSlotByte = slot;
	pkt->mContainerIdByte = 0;

	Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->send(pkt);
	Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(pkt);

	return true;
}

int getCrystal() {
	auto* plrInv = Game.getLocalPlayer()->getplayerInventory();
	auto* inv = plrInv->container;

	for (auto i = 0; i < 9; i++) {
		auto* itemStack = inv->getItem(i);

		if (itemStack->isValid() && itemStack->item->gettexture_name() == "end_crystal") {
			return i;
		}
	}
	return plrInv->selectedSlot;
}
void AutoCrystal::onNormalTick(LocalPlayer* localPlayer) {
	targetList.clear();
	entityList.clear();
	placeList.clear();
	breakList.clear();
	if (!multiTask && Game.getLocalPlayer()->getItemUseDuration() > 0)
		return;
	Vec3 localPos = Game.getLocalPlayer()->getEyePos();
	for (Actor* actor : Game.getLocalPlayer()->getlevel()->getRuntimeActorList()) {
		if (!actor) continue;
		entityList.push_back(actor);
		if (!TargetUtil::isTargetValid(actor)) continue;
		if (actor->getEyePos().dist(localPos) > targetRange) continue;

		targetList.push_back(actor);
	}
	if (selfTest) targetList.push_back(Game.getLocalPlayer());
	if (targetList.empty()) return;
	std::sort(targetList.begin(), targetList.end(), TargetUtil::sortByDist);
	if (place) generatePlacement(targetList[0]);
	if (explode) getCrystals(targetList[0]);
	const int oldSlot = InventoryUtils::getSelectedSlot();
	bool didSwitch = false;
	if (logic == 1) {
		if (swap) InventoryUtil::switchSlot(getCrystal());
		if(explode)breakCrystal(Game.getLocalPlayer()->gameMode);
        if(place)placeCrystal(Game.getLocalPlayer()->gameMode);
		if (predict)predictEndCrystal(Game.getLocalPlayer()->gameMode);
		if (switchBack) InventoryUtil::switchSlot(oldSlot);
	}
	if (logic == 0) {
		if (swap) InventoryUtil::switchSlot(getCrystal());
		if(place)placeCrystal(Game.getLocalPlayer()->gameMode);
		if(explode)breakCrystal(Game.getLocalPlayer()->gameMode);
		if (predict)predictEndCrystal(Game.getGameMode());
		if (switchBack) InventoryUtil::switchSlot(oldSlot);
	}
}
void AutoCrystal::onSendPacket(Packet* packet) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (rotate && !placeList.empty()) {
		const Vec2<float>& angle = Game.getLocalPlayer()->getEyePos().CalcAngle(placeList[0].blockPos.toFloat());
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



void AutoCrystal::onD2DRender() {
	if (renderType == 0 || placeList.empty()) return;

	int maxRender = std::min((int)placeList.size(), wasteAmount);
	for (int i = 0; i < maxRender; i++) {
		const CrystalPlace& place = placeList[i];
		AABB blockAABB;

		if (renderType == 1) {
			blockAABB.lower = Vec3<float>(place.blockPos.x, place.blockPos.y, place.blockPos.z);
			blockAABB.upper = blockAABB.lower.add(Vec3<float>(1.f, 1.f, 1.f));
		}
		else if (renderType == 2) {
			blockAABB.lower = Vec3<float>(place.blockPos.x, place.blockPos.y + 1.f, place.blockPos.z);
			blockAABB.upper = blockAABB.lower.add(Vec3<float>(1.f, 0.f, 1.f)); // Flat
		}

		D2D::drawBox(blockAABB, renderColor, lineColor, 1.f, true, true);
	}
}
