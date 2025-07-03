#include "AutoCrystal.h"
#include "../Player/PacketMine.h"
#include "../../../Client.h"
#include <algorithm>
#include <vector>
#include <ranges>
#include <mutex>
#include <omp.h>
bool Damagerender = false;
int placeProximity = 4;
int extrapolateAmount = 0;
int idapro = 0;
bool userender = false;
bool shouldSilentSwitch = true;
AutoCrystal::AutoCrystal() : Module("AutoCrystal", "NULL", Category::COMBAT, 0x0) {
	slider<float>("Target Range", "Maximum distance to detect targets.", ValueType::FLOAT_T, &targetRange, 5.f, 20.f);
	boolsetting("Auto Place", "Automatically places crystals when conditions are met.", &autoPlace);
	slider<float>("Place Range", "Maximum distance at which crystals can be placed.", ValueType::FLOAT_T, &placeRange, 3.f, 10.f);
	slider<float>("Max Place Damage", "Maximum potential damage allowed when placing a crystal.", ValueType::FLOAT_T, &maxPlaceDame, 1.f, 20.f);
	slider<float>("Min Place Damage", "Minimum required damage for placing a crystal.", ValueType::FLOAT_T, &minPlaceDame, 1.f, 20.f);
	slider<int>("Place Delay", "Time (in ticks) between crystal placements.", ValueType::INT_T, &placeDelay, 0, 20);

	boolsetting("Auto Break", "Automatically breaks crystals when conditions are met.", &autoBreak);
	slider<float>("Break Range", "Maximum distance at which crystals can be broken.", ValueType::FLOAT_T, &breakRange, 3.f, 10.f);
	slider<float>("Max Break Damage", "Maximum potential damage allowed when breaking a crystal.", ValueType::FLOAT_T, &maxBreakDame, 1.f, 20.f);
	slider<float>("Min Break Damage", "Minimum required damage for breaking a crystal.", ValueType::FLOAT_T, &minBreakDame, 1.f, 20.f);
	slider<int>("Break Delay", "Time (in ticks) between breaking crystals.", ValueType::INT_T, &breakDelay, 0, 20);

	boolsetting("Predict", "Predicts where to attack ahead of time.", &idPredict);
	slider<int>("Packets", "Number of packets sent for crystal actions.", ValueType::INT_T, &packets, 1, 30);
	slider<int>("Packet Attempts", "Number of packets in onNormalTick", ValueType::INT_T, &idapro, 1, 5);
	slider<int>("Ticks", "Number of game ticks to simulate for prediction.", ValueType::INT_T, &Ticks, 0, 100);
	slider<int>("Send Delay", "Delay (in ticks) between sending packets.", ValueType::INT_T, &sendDelay, 0, 20);
	boolsetting("Mobs", "Allows testing on mobs instead of players.", &Mob);

	slider<int>("Extrapolate", "Predicts movement based on previous data (tick prediction).", ValueType::INT_T, &extrapolateAmount, 0, 10);
	slider<int>("Place Proximity", "Defines how close a crystal must be to another for placement.", ValueType::INT_T, &placeProximity, 1, 7);

	boolsetting("Render", "Toggles visual rendering of crystals and related effects.", &userender);
	boolsetting("Render Damages", "Renders damage values above crystals.", &Damagerender);

	boolsetting("Silent Switch", "Toggles hotbar spoofing.", &shouldSilentSwitch);
}
std::string AutoCrystal::getModName() {
	
	return names;
	
}

bool AutoCrystal::sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2) {
	return a1.TgDameTake > a2.TgDameTake;
}

bool AutoCrystal::sortEntityByDist(Actor* a1, Actor* a2) {
	auto localPlayerPos = mc.getLocalPlayer()->stateVectorComponent->pos;
	return (a1->stateVectorComponent->pos.dist(localPlayerPos) < a2->stateVectorComponent->pos.dist(localPlayerPos));
}

bool AutoCrystal::isHoldingCrystal() {
	auto* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	auto* inv = plrInv->inventory;
	auto slot = plrInv->selectedSlot;
	auto* itemStack = inv->getItemStack(slot);
	return (itemStack->isValid()) ? itemStack->getItemPtr()->getitemId() == 720 : false;
}
void AutoCrystal::cleardalist() {
	entityList.clear();
	targetList.clear();
	placeList.clear();
	breakList.clear();
}
int dam;
void AutoCrystal::onEnable() {
	cleardalist();
	placerot = false;
	breakrot = false;
	placing = false;
	breaking = false;
}

void AutoCrystal::onDisable() {
	cleardalist();
	placerot = false;
	breakrot = false;
	placing = false;
	breaking = false;
}
bool AutoCrystal::canPlaceCrystal(Vec3<int> placePos) {
	auto* lp = mc.getLocalPlayer();
	auto* region = lp->dimension->blockSource;
	auto isAir = [](Block* b) { return b && b->blockLegacy->blockName == "air"; };
	auto isObsidianOrBedrock = [](Block* b) { return b && (b->blockLegacy->blockName == "obsidian" || b->blockLegacy->blockName == "bedrock"); };
	if (!isObsidianOrBedrock(region->getBlock(placePos))) return false;
	if (!isAir(region->getBlock(placePos.add(0, 1, 0))) || !isAir(region->getBlock(placePos.add(0, 2, 0)))) return false;
	if (lp->stateVectorComponent->pos.dist(placePos.toFloat().add(0.5f, 0.f, 0.5f)) > placeRange) return false;
	AABB placeAABB{ placePos.toFloat().add(0.f, 1.f, 0.f), placePos.toFloat().add(1.f, 2.f, 1.f) };
	for (auto* actor : entityList) {
		if (actor->getEntityTypeId() == 71);
			continue;
		AABB targetAABB = *actor->getAABB();
		if (actor->getEntityTypeId() == 319) { 
			targetAABB.lower = targetAABB.lower.sub(Vec3<float>(0.1f, 0.f, 0.1f));
			targetAABB.upper = targetAABB.upper.add(0.1f, 0.f, 0.1f);
		}
		if (targetAABB.intersects(placeAABB))
			return false;
	}

	return true;
}

void AutoCrystal::generatePlacement(Actor* target) {
	auto* lp = mc.getLocalPlayer();
	auto* region = lp->dimension->blockSource;
	const int radius = static_cast<int>(placeProximity); 
	const int maxY = 7;
	auto targetPos = target->stateVectorComponent->pos;
	auto targetVel = target->stateVectorComponent->velocity;

	for (int x = -radius; x <= radius; x++) {
		for (int y = -maxY; y <= 3; y++) {
			for (int z = -radius; z <= radius; z++) {
				Vec3<int> blockPos(
					static_cast<int>(targetPos.x + (targetVel.x * extrapolateAmount)) + x,
					static_cast<int>(targetPos.y + (targetVel.y * extrapolateAmount)) + y,
					static_cast<int>(targetPos.z + (targetVel.z * extrapolateAmount)) + z
				);

				if (canPlaceCrystal(blockPos)) {
					CrystalPlacement placement(blockPos, target);
					if (placement.LpDameTake < maxPlaceDame && placement.TgDameTake >= minPlaceDame) {
						std::lock_guard<std::mutex> lock(placeListMutex);
						placeList.push_back(placement);
					}
				}
			}
		}
	}

	std::sort(placeList.begin(), placeList.end(), AutoCrystal::sortCrystalByTargetDame);
}



void AutoCrystal::getCrystalActorList(Actor* target) {
	auto* localPlayer = mc.getLocalPlayer();
	std::vector<CrystalBreaker> localBreakList;

	for (auto* ent : entityList) {
		if (ent->getEntityTypeId() != 71 || ent->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) > breakRange)
			continue;

		CrystalBreaker cBreaker(ent, target);
		if (cBreaker.LpDameTake < maxBreakDame && cBreaker.TgDameTake >= minBreakDame)
			localBreakList.push_back(cBreaker);
	}

	{
		std::lock_guard<std::mutex> lock(breakListMutex);
		breakList.insert(breakList.end(), localBreakList.begin(), localBreakList.end());
		std::sort(breakList.begin(), breakList.end(), AutoCrystal::sortCrystalByTargetDame);
	}
}

void AutoCrystal::placeCrystal() {
	std::lock_guard<std::mutex> lock(placeListMutex);

	if (placeList.empty() || !isHoldingCrystal())
		return;

	auto* gm = mc.getGameMode();

	if (placeDelayTick >= placeDelay) {
		for (CrystalPlacement& placement : placeList) {
			auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(placement.placePos.toFloat());
			rotAnglePlace = rotationToPlacement;
			placerot = true;
			mc.getLocalPlayer()->swing();
			//InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), placement.placePos.toFloat());
			gm->buildBlock(placement.placePos, Math::random(1, 2), false);
			//mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
			dam = placement.TgDameTake;
			
			break; 
		}

		placeDelayTick = 0;
	}
	else {
		rotAnglePlace = {};
		
		placeDelayTick++;
	}
}
void attack(Actor* target) {
	auto lp = mc.getLocalPlayer();
	if (!mc.getGameMode()->attack(target))  
		return;
	lp->swing();
//	Game.cpsLeft.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}
void AutoCrystal::breakCrystal() {
	std::lock_guard<std::mutex> lock(breakListMutex);

	if (!autoBreak || breakList.empty())
		return;

	auto* gm = mc.getGameMode();

	if (breakDelayTick >= breakDelay) {
		highestID = breakList[0].crystalActor->getRuntimeID();

		
		
		attack(breakList[0].crystalActor);

		breakDelayTick = 0;
	}
	else {
		rotAngleBreak = {};
		
		breakDelayTick++;
	}
}
void AutoCrystal::breakIdPredictCrystal() {
	std::lock_guard<std::mutex> lock(breakListMutex);
	if (!autoBreak || placeList.empty())
		return;
	auto* gm = mc.getGameMode();
	int realPacket = packets * 5; 
	if (sendDelayTick >= sendDelay) {
		shouldChangeID = true;
		if (idPredict)
			highestID = placeList[0].targetActor->getRuntimeID();
		for (int i = 0; i < realPacket; i++) {
			InteractPacket inter(InteractAction::LEFT_CLICK, highestID,
				placeList[0].targetActor->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));
			if (!breakList.empty())
				attack(breakList[0].crystalActor);
			mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
			if (idPredict)
				highestID++; 
		}
		if (idPredict)
			highestID -= realPacket; 
		shouldChangeID = false;
		sendDelayTick = 0;
	}
	else {
		sendDelayTick++;
		highestID++;
	}
}
int getCrystal() {
	auto* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	auto* inv = plrInv->inventory;

	for (auto i = 0; i < 9; i++) {
		auto* itemStack = inv->getItemStack(i);

		if (itemStack->isValid() && itemStack->getItemPtr()->gettexture_name() == "end_crystal") {
			return i;
		}
	}
	return plrInv->selectedSlot;
}
#include <string> // for string and to_string()
void AutoCrystal::onNormalTick(Actor* actor) {
	cleardalist();
	auto* localPlayer = mc.getLocalPlayer();
	auto* level = localPlayer->getLevel();
	static PacketMine* blockReachMod = (PacketMine*)client->moduleMgr->getModule("PacketMine");
	if (!level || !localPlayer->dimension->blockSource) return;
	auto* gm = mc.getGameMode();
	auto* plrInv = localPlayer->getPlayerInventory();
	auto* inv = plrInv->inventory;
	auto* itemStack = localPlayer->getCarriedItem();
	auto* item = itemStack->getItemPtr();
	if (item != nullptr && item->isFood()) {
		if (localPlayer->getItemUseDuration() > 0) return;
	}
	if (blockReachMod->getIsMining()) return;
	int initialCrystalsAmount = inv->getItemStack(getCrystal())->stackCount;

	for (auto* target : level->getRuntimeActorList()) {
		entityList.push_back(target);
		if (TargetUtils::isTargetValid(target, Mob) &&
			localPlayer->stateVectorComponent->pos.dist(target->stateVectorComponent->pos) < targetRange) {
			targetList.push_back(target);
		}
	}

	if (targetList.empty()) return;

	std::sort(targetList.begin(), targetList.end(), AutoCrystal::sortEntityByDist);
	float distance = 0;
	std::string namess;
	if (autoPlace) generatePlacement(targetList[0]);
	if (localPlayer->stateVectorComponent->pos.dist(targetList[0]->stateVectorComponent->pos) < targetRange)
	{
		distance = localPlayer->stateVectorComponent->pos.dist(targetList[0]->stateVectorComponent->pos);

	}
	if (targetList.empty())
	{
		namess = "";
	}
	else if (!targetList.empty())
	{
		namess = targetList[0]->getNameTag()->c_str();
	}
	else if (localPlayer->stateVectorComponent->pos.dist(targetList[0]->stateVectorComponent->pos) > targetRange)
	{
		distance = 0;
	}
	currenttarget = targetList[0];
	names = std::string("[") + std::string(std::to_string(highestID)) + std::string("]");
	if (autoBreak) getCrystalActorList(targetList[0]);

	auto bestSlot = getCrystal();
	auto oldSlot = plrInv->selectedSlot;
	auto shouldSwitch = (bestSlot != plrInv->selectedSlot);
	if (shouldSwitch) {
		plrInv->selectedSlot = bestSlot;
	}
	
	if (isHoldingCrystal()) {
		crystalSpeed = dam;
		prevCrystalsAmount = inv->getItemStack(plrInv->selectedSlot)->stackCount;
		localPlayer->addExperience(1);
		placeCrystal();
		breakCrystal();
		for (int i = 0; i < idapro; i++) {
			if (idPredict) breakIdPredictCrystal();
		}
	}
	if (shouldSilentSwitch) plrInv->selectedSlot = oldSlot;
}


/*	/*if (packet->getId() == PacketID::PlayerAuthInput || packet->getId() == PacketID::MovePlayerPacket) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		authPacket->ticksAlive = Ticks;
		movePacket->tick = Ticks;
		movePacket->actorRuntimeID = mc.getLocalPlayer()->getRuntimeID();
		authPacket->rotation = rotAnglePlace;
		movePacket->rotation = rotAnglePlace;
		authPacket->headYaw = rotAnglePlace.y;
		movePacket->headYaw = rotAnglePlace.y;

			if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}*/
void AutoCrystal::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rotation = rotAnglePlace;
		authPacket->headYaw = rotAnglePlace.y;
	}

	/*
	}

	if (!shouldChangeID) return;

	if (packet->getId() == PacketID::InventoryTransaction) {
		auto* invPacket = reinterpret_cast<InventoryTransactionPacket*>(packet);
		auto* invComplex = invPacket->transaction.get();

		if (invComplex->type == ComplexInventoryTransaction::Type::ItemUseOnEntityTransaction) {
			*(int*)((uintptr_t)(invComplex)+0x68) = highestID;
		}
	}
	else if (packet->getId() == PacketID::LevelSoundEvent) {
		shouldCancel = true;
	}*/
}

void AutoCrystal::onRender(MinecraftUIRenderContext* renderCtx) {
	if (userender) {
		int placed = 0;
		for (CrystalPlacement& placement : placeList) {
			Vec3<float> drawboxCenter = placement.placePos.toFloat().add(0.5f, 1.5f, 0.5f);
			static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
			UIColor mainColor(colorsMod->getColor());
			RenderUtils::drawBox(placement.placePos.toFloat(), UIColor(mainColor.r, mainColor.g, mainColor.b, 42), UIColor(mainColor.r, mainColor.g, mainColor.b, 255), 0.3f, true, false);
			placed++;

			if (placed >= multiPlace) break;
		}
	}
}
void AutoCrystal::onImGuiRender(ImDrawList* d) {
	if (userender) {
		LocalPlayer* lp = mc.getLocalPlayer();
		if (lp == nullptr) return;
		if (lp->getLevel() == nullptr) return;
		if (!mc.getClientInstance()->minecraftGame->canUseKeys) return;
		static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
		UIColor mainColor = colorsMod->getColor();
		int placed = 0;
		for (CrystalPlacement& placement : placeList) {
			Vec2<float> pos;
			if (ImGuiUtils::worldToScreen(placement.placePos.toFloat().add(0.f, 0.5f, 0.f), pos)) {
				float dist = placement.placePos.toFloat().dist(mc.getLocalPlayer()->stateVectorComponent->pos);

				float size = fmax(0.65f, 3.f / dist);
				if (size > 2.f) size = 2.f;
				std::string name = std::to_string(dam);
				name = Utils::sanitize(name);
				float textSize = 2.5f * size;
				float textWidth = ImGuiUtils::getTextWidth(name, textSize);
				float textHeight = ImGuiUtils::getTextHeight(textSize);
				Vec2<float> textPos = Vec2<float>(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);

				if (Damagerender) {
					ImGuiUtils::drawText(textPos, name, UIColor(255, 255, 255, 255), textSize, true);
				}
			}
			// Break loop after rendering the current placement
			break;
		}
	}
}
