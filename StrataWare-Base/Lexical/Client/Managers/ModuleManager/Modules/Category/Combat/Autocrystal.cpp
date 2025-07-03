#include "AutoCrystal.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
std::chrono::steady_clock::time_point lastPlaceTime;
std::chrono::steady_clock::time_point lastBreakTime;
std::chrono::steady_clock::time_point lastBoostTime;
int placeDelay;
int breakDelay;
int boostDelay;
bool shouldBreakCrystals = false;
std::vector<Actor*> validCrystals;
int breakAttempts = 0;
int maxBreakAttempts = 3;
AutoCrystal::AutoCrystal() : Module("AutoCrystal", "NCP Boze 2.0 Placements", Category::COMBAT) {
	// PLACE SETTINGSregisterSetting(new ToggleSetting("Safety", "Prioritizes safety over damage", &safety, true));
	registerSetting(new ToggleSetting("Safety", "Prioritizes safety over damage", &safety, true));
	registerSetting(new ToggleSetting("Java", "For java servers", &java, false));
	registerSetting(new ToggleSetting("Place", "Place End Crystals at Target", &place, true));
	registerSetting(new NumberSetting<int>("PlaceDist", "Range for placing crystals", &placeRange, 5, 1, 12));
	registerSetting(new NumberSetting<int>("PlaceDelay", "Delay in milliseconds for placing crystals", &placeDelay, 100, 0, 1000));
	registerSetting(new NumberSetting<int>("WasteAmount", "Number of crystals to place", &wasteAmount, 3, 1, 10));
	registerSetting(new ToggleSetting("Rotate", "Rotate to placement locations", &rotate, true));

	// BREAK SETTINGS
	registerSetting(new ToggleSetting("Break", "Explode End Crystals at Target", &explode, true));
	registerSetting(new NumberSetting<int>("BreakDist", "Range for breaking crystals", &breakRange, 5, 1, 12));
	registerSetting(new NumberSetting<int>("BreakDelay", "Delay in milliseconds for breaking crystals", &breakDelay, 100, 0, 1000));
	registerSetting(new NumberSetting<int>("MaxBreakAttempts", "Max breaking.", &maxBreakAttempts, 5, 1, 12));

	// BOOST / PREDICTION SETTINGS
	registerSetting(new ToggleSetting("Boost", "Predict crystal runtime ID for faster actions", &predict, false));
	registerSetting(new NumberSetting<int>("BoostDelay", "Delay in milliseconds for prediction boost", &boostDelay, 100, 0, 1000));
	registerSetting(new NumberSetting<int>("Packets", "Number of packets for prediction", &predictPacket, 5, 1, 10));
	registerSetting(new NumberSetting<float>("BoostDamage", "Minimum damage for boosting", &boostDmg, 10.f, 0.f, 20.f));

	// MISC SETTINGS
	registerSetting(new ToggleSetting("MultiTask", "Multitasks like eating and crystalling", &multiTask, true));
	registerSetting(new NumberSetting<int>("TargetDist", "Range for targeting entities", &targetRange, 10, 1, 20));
	registerSetting(new NumberSetting<float>("Proximity", "Proximity for crystal placement", &proximity, 6.f, 1.f, 12.f));
	registerSetting(new NumberSetting<float>("EnemyDmg", "Minimum damage to enemy", &enemyDamage, 8.f, 0.f, 36.f));
	registerSetting(new NumberSetting<float>("SelfDmg", "Maximum damage to self", &localDamage, 4.f, 0.f, 36.f));
	registerSetting(new ToggleSetting("Swap", "Swap to end crystal", &swap, true));
	registerSetting(new ToggleSetting("SwitchBack", "Switch back to previous slot", &switchBack, true));
	registerSetting(new ToggleSetting("SelfTest", "Enable testing on yourself", &selfTest, false));

	// RENDER SETTINGS
	registerSetting(new EnumSetting("Render", "Rendering mode for placements", { "Off", "Box", "Flat" }, &renderType, 0));
	registerSetting(new ColorSetting("Color", "Render color", &renderColor, { 255, 0, 0 }));
	registerSetting(new ToggleSetting("RenderDamage", "Display damage dealt during render", &dmgText, true));
}


bool AutoCrystal::sortCrystal(CrystalData c1, CrystalData c2) {
	return c1.targetDamage > c2.targetDamage;
}
/* BOZE START */
float calculateDamageToTarget(Actor* crystal, Actor* target) {
	float distance = crystal->getPos().dist(target->getPos());
	return std::max(0.0f, 12.0f - (distance * 2.5f));
}
float calculateDamageToSelf(Actor* crystal) {
	float distance = crystal->getPos().dist(Game::ourPlayer()->getPos());
	return std::max(0.0f, 12.0f - (distance * 2.5f));
}
/* BOZE END */

bool AutoCrystal::isPlaceValid(const BlockPos& blockPos, Actor* actor) {
	int blockId = Game::clientInstance->getRegion()->getBlock(blockPos)->blockLegacy->blockId;
	if (blockId != 7 && blockId != 49) return false;
	if (!java) {
		Vec3<int>checkPos(blockPos.x, blockPos.y + 2, blockPos.z);
		int blockId22 = Game::clientInstance->getRegion()->getBlock(checkPos)->blockLegacy->blockId;
		if (blockId22 != 0) return false;
	}
	Vec3 targetCenter(blockPos.x + 0.5f, blockPos.y + 1.0f, blockPos.z + 0.5f);
	if (Game::ourPlayer()->getEyePos().dist(targetCenter) > placeRange) return false;
	AABB crystalAABB(
		Vec3<float>(blockPos.x, blockPos.y, blockPos.z),
		Vec3<float>(blockPos.x + 1.0f, blockPos.y + 2.0f, blockPos.z + 1.0f)
	);
	for (Actor* entity : entityList) {
		if (entity->getActorTypeComponent()->id == 71) continue;
		AABB entityAABB = entity->aabbShape->aabb;
		if (entity->getActorTypeComponent()->id == 319 || entity == Game::ourPlayer()) {
			entityAABB = AABB(
				Vec3(entity->getHumanPos().x - 0.3f, entity->getHumanPos().y, entity->getHumanPos().z - 0.3f),
				Vec3(entity->getHumanPos().x + 0.3f, entity->getHumanPos().y + 1.8f, entity->getHumanPos().z + 0.3f)
			);
		}
		if (entityAABB.intersects(crystalAABB)) {
			return false;
		}
	}
	return true;
}

void AutoCrystal::generatePlacement(Actor* actor) {
	placeList.clear();
	const int radius = static_cast<int>(proximity);
	Vec3<float> targetPos = actor->getPos();
	targetPos.y -= 1.6f;
	for (int x = -radius; x <= radius; x++) {
		for (int y = -2; y <= 2; y++) {
			for (int z = -radius; z <= radius; z++) {
				BlockPos blockPos(
					static_cast<int>(targetPos.x) + x,
					static_cast<int>(targetPos.y) + y,
					static_cast<int>(targetPos.z) + z
				);

				if (isPlaceValid(blockPos, actor)) {
					placeList.emplace_back(CrystalPlace(actor, blockPos));
				}
			}
		}
	}
	std::sort(placeList.begin(), placeList.end(), [](const CrystalPlace& a, const CrystalPlace& b) {
		return a.targetDamage > b.targetDamage;
		});
}
void AutoCrystal::getCrystals(Actor* actor) {
	for (Actor* entity : entityList) {
		if (entity == nullptr) continue;
		if (entity->getActorTypeComponent()->id != 71) continue;
		if (entity->getEyePos().dist(Game::ourPlayer()->getEyePos()) > breakRange) continue;
		const CrystalBreak breakment(actor, entity);
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
	if (Game::ourPlayer() == nullptr) return;
	if (placeList.empty()) return;
	if (InventoryUtils::getSelectedItemId() != 758 && !switchBack) return;
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPlaceTime).count();
	if (duration >= placeDelay) {
		int placed = 0;
		for (const CrystalPlace& place : placeList) {
			if (placeList[0].targetDamage < enemyDamage) return;
			gm->buildBlock(place.blockPos, 0, false);
			if (++placed >= wasteAmount) break;
		}
		lastPlaceTime = now;
	}
	if (!placeList.empty() && predict) {
		if (placeList[0].targetDamage < boostDmg) return;
		const int realPacket = predictPacket * 5;
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBoostTime).count();
		if (duration >= boostDelay) {
			shouldChange = true;
			for (int i = 0; i < realPacket; i++) {
				gm->attack(placeList[0].actor);

				highestId++;
			}
			highestId -= realPacket;
			shouldChange = false; 
			lastBoostTime = now;
		}
	}
}
/* BOZE START */
void AutoCrystal::breakCrystal(GameMode* gm) { //i just bozed
	if (Game::ourPlayer() == nullptr || !explode) return;
	validCrystals.clear();
	for (Actor* entity : entityList) {
		if (entity == nullptr || entity->getActorTypeComponent()->id != 71)
			continue;
		if (entity->getEyePos().dist(Game::ourPlayer()->getEyePos()) > breakRange)
			continue;
		float targetDmg = calculateDamageToTarget(entity, targetList[0]);
		float selfDmg = calculateDamageToSelf(entity);
		if (targetDmg >= enemyDamage && (!safety || selfDmg <= localDamage)) {
			validCrystals.push_back(entity);
		}
	}
	std::sort(validCrystals.begin(), validCrystals.end(), [this](Actor* a, Actor* b) {
		return calculateDamageToTarget(a, targetList[0]) > calculateDamageToTarget(b, targetList[0]);
		});
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBreakTime).count();

	if (!validCrystals.empty() && duration >= breakDelay && breakAttempts < maxBreakAttempts) {
		Actor* bestCrystal = validCrystals[0];
		gm->attack(bestCrystal);

		lastBreakTime = now;
		breakAttempts++;
		shouldBreakCrystals = true;
	}
	else {
		shouldBreakCrystals = false;
		breakAttempts = 0;
	}
}/* BOZING IT!!~ */

void AutoCrystal::onNormalTick(LocalPlayer* localPlayer) {
	targetList.clear();
	entityList.clear();
	placeList.clear();
	breakList.clear();
	if (!multiTask && Game::ourPlayer()->getItemUseDuration() > 0) return;
	for (Actor* actor : Game::ourPlayer()->getlevel()->getRuntimeActorList()) {
		if (!actor) continue;
		entityList.push_back(actor);
		if (!TargetUtil::isTargetValid(actor)) continue;
		if (actor->getEyePos().dist(Game::ourPlayer()->getEyePos()) > targetRange) continue;
		targetList.push_back(actor);
	}
	if (selfTest) targetList.push_back(Game::ourPlayer());
	if (targetList.empty()) return;
	std::sort(targetList.begin(), targetList.end(), TargetUtil::sortByDist);
	if (place) generatePlacement(targetList[0]);
	if (explode) getCrystals(targetList[0]);
	const int oldSlot = InventoryUtils::getSelectedSlot();
	if (swap) InventoryUtils::SwitchTo(InventoryUtils::getItem(758));
	if (place) placeCrystal(Game::ourPlayer()->gameMode);
	if (explode) breakCrystal(Game::ourPlayer()->gameMode);
	if (switchBack) InventoryUtils::SwitchTo(oldSlot);
}

bool AutoCrystal::onSendPacket(Packet* packet) {
	if (Game::ourPlayer() == nullptr) {
	}
	else if (rotate && !placeList.empty()) {
		const Vec2<float>& angle = Game::ourPlayer()->getEyePos().CalcAngle(placeList[0].blockPos.toFloat());

		if (packet->getId() == PacketID::PlayerAuthInput) {
			PlayerAuthInputPacket* authPkt = (PlayerAuthInputPacket*)packet;
			authPkt->rotation = angle;
			authPkt->headYaw = angle.y;
		}
		if (predict || shouldChange) {
			if (packet->getId() == PacketID::InventoryTransaction) {
			}
		}
	}
	return false;
}
struct CrystalRenderState {
	Vec3<float> startPos;
	Vec3<float> targetPos;
	float startTime = -1.0f;
	float duration = 0.3f;
	bool isFadingOut = false;
};

float getTime() {
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = now - start;
	return elapsed.count();
}

std::unordered_map<Vec3<int>, CrystalRenderState> animationStates;

void AutoCrystal::onLevelRender() {
	if (renderType == 0 || placeList.empty()) return;

	UIColor baseColor(renderColor.r, renderColor.g, renderColor.b, renderColor.a);
	float currentTime = getTime();
	std::unordered_set<Vec3<int>> currentBlocks;
	size_t renderCount = std::min((size_t)wasteAmount, placeList.size());

	for (size_t i = 0; i < renderCount; i++) {
		const CrystalPlace& place = placeList[i];
		Vec3<int> blockPos = place.blockPos;
		currentBlocks.insert(blockPos);

		Vec3<float> blockPosF((float)blockPos.x, (float)blockPos.y, (float)blockPos.z);
		auto& state = animationStates[blockPos];

		if (state.startTime < 0.0f) {
			state.startPos = blockPosF;
			state.targetPos = blockPosF;
			state.startTime = currentTime;
			state.isFadingOut = false;
		}
		else if (state.targetPos.x != blockPosF.x || state.targetPos.y != blockPosF.y || state.targetPos.z != blockPosF.z) {
			state.startPos = state.targetPos;
			state.targetPos = blockPosF;
			state.startTime = currentTime;
			state.isFadingOut = false;
		}

		float t = (currentTime - state.startTime) / state.duration;
		t = std::clamp(t, 0.0f, 1.0f);
		float easedT = t * t * (3.0f - 2.0f * t);

		Vec3<float> interpPos;
		interpPos.x = state.startPos.x + (state.targetPos.x - state.startPos.x) * easedT;
		interpPos.y = state.startPos.y + (state.targetPos.y - state.startPos.y) * easedT;
		interpPos.z = state.startPos.z + (state.targetPos.z - state.startPos.z) * easedT;

		AABB box(
			Vec3<float>(interpPos.x, interpPos.y, interpPos.z),
			Vec3<float>(interpPos.x + 1.0f, interpPos.y + 1.0f, interpPos.z + 1.0f)
		);
		AABB boxFlat(
			Vec3<float>(interpPos.x, interpPos.y + 0.8f, interpPos.z),
			Vec3<float>(interpPos.x + 1.0f, interpPos.y + 1.0f, interpPos.z + 1.0f)
		);

		UIColor fadeColor = baseColor;
		fadeColor.a = static_cast<unsigned char>(baseColor.a * easedT);

		if (renderType == 1) {
			MCR::drawBox3dFilled(box, fadeColor, UIColor(0, 0, 0, 0), easedT);
		}
		else if (renderType == 2) {
			MCR::drawBox3dFilled(boxFlat, fadeColor, UIColor(0, 0, 0, 0), easedT);
		}
	}
	for (auto it = animationStates.begin(); it != animationStates.end(); ) {
		const Vec3<int>& blockPos = it->first;
		CrystalRenderState& state = it->second;

		if (currentBlocks.count(blockPos)) {
			++it;
			continue;
		}

		if (!state.isFadingOut) {
			state.startPos = state.targetPos;
			state.startTime = currentTime;
			state.isFadingOut = true;
		}

		float t = (currentTime - state.startTime) / state.duration;
		t = std::clamp(t, 0.0f, 1.0f);
		float easedT = t * t * (3.0f - 2.0f * t);
		float fadeAlpha = 1.0f - easedT;

		Vec3<float> pos = state.startPos;

		AABB box(
			Vec3<float>(pos.x, pos.y, pos.z),
			Vec3<float>(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f)
		);
		AABB boxFlat(
			Vec3<float>(pos.x, pos.y + 0.8f, pos.z),
			Vec3<float>(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f)
		);

		UIColor fadeColor = baseColor;
		fadeColor.a = static_cast<unsigned char>(baseColor.a * fadeAlpha);

		if (renderType == 1) {
			MCR::drawBox3dFilled(box, fadeColor, UIColor(0, 0, 0, 0), fadeAlpha);
		}
		else if (renderType == 2) {
			MCR::drawBox3dFilled(boxFlat, fadeColor, UIColor(0, 0, 0, 0), fadeAlpha);
		}

		if (t >= 1.0f) {
			it = animationStates.erase(it);
		}
		else {
			++it;
		}
	}
}
