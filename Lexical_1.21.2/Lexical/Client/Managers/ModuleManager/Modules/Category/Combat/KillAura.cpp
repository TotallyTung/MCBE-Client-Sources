#include "KillAura.h"
#include "../../../../../../SDK/Network/Packet/PlayerActionPacket.h"
int seleelc = 0;
KillAura::KillAura() : Module("KillAura", "Attacks entities around you", Category::COMBAT) {
	registerSetting(new PageSetting("Page", "Module Page", { "Target", "Attack", "Weapon" }, &seleelc));
	this->modulePagePtr = &seleelc;
	registerSetting(new SliderSetting<float>("Range", "Range in which targets will be hit", &range, 5.f, 3.f, 40.f, 0));
	registerSetting(new SliderSetting<float>("WallRange", "Range in which targets will be hit through walls", &wallRange, 0.f, 0.f, 40.f, 0));
	registerSetting(new BoolSetting("Mobs", "Attack Mobs", &includeMobs, false, 0));

	registerSetting(new SliderSetting<int>("Interval", "Attack delay", &interval, 1, 0, 20, 1));
	registerSetting(new SliderSetting<int>("Multiplier", "Number of attacks per tick", &multiplier, 1, 1, 5, 1));
	registerSetting(new EnumSetting("Rotations", "Rotates to the targets", { "None", "Silent", "Strafe", "Short" }, &rotMode, 1, 1));
	registerSetting(new EnumSetting("HitType", "Hit mode", { "Single", "Multi" }, &hitType, 0, 1));
	registerSetting(new SliderSetting<int>("HitChance", "Chance to hit", &hitChance, 100, 0, 100, 1));

	registerSetting(new EnumSetting("Weapon", "Auto switch to best weapon", { "None", "Switch", "Spoof" }, &autoWeaponMode, 0, 2));
}


std::string KillAura::getModeText() {
	return hitType == 0 ? "Single" : "Multi";
}

void KillAura::onDisable() {
	targetList.clear();
	oTick = INT_MAX;
	shouldRot = false;
}

void KillAura::onEnable() {
	targetList.clear();
	oTick = INT_MAX;
	shouldRot = false;
}

bool KillAura::sortByDist(Actor* a1, Actor* a2) {
	Vec3<float> lpPos = Game::getLocalPlayer()->getPos();
	return a1->getPos().dist(lpPos) < a2->getPos().dist(lpPos);
}

int KillAura::getBestWeaponSlot(Actor* target) {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	Container* inventory = localPlayer->playerInventory->container;
	float bestDamage = 0.f;
	int bestSlot = localPlayer->playerInventory->selectedSlot;

	for (int i = 0; i < 9; i++) {
		localPlayer->playerInventory->selectedSlot = i;
		float dmg = localPlayer->calculateAttackDamage(target);
		if (dmg > bestDamage) {
			bestDamage = dmg;
			bestSlot = i;
		}
	}

	return bestSlot;
}

void KillAura::Attack(Actor* target) {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (rand() % 100 < hitChance) {
		localPlayer->gameMode->attack(target);
	}
	localPlayer->swing();
}

void KillAura::onNormalTick(LocalPlayer* localPlayer) {
	Level* level = localPlayer->level;
	BlockSource* region = Game::clientInstance->getRegion();
	targetList.clear();

	for (auto& entity : level->getRuntimeActorList()) {
		if (TargetUtil::isTargetValid(entity, includeMobs)) {
			float checkRange = (region->getSeenPercent(localPlayer->getEyePos(), entity->aabbShape->aabb) == 0.0f)
				? wallRange : range;
			if (WorldUtil::distanceToEntity(localPlayer->getPos(), entity) <= checkRange)
				targetList.push_back(entity);
		}
	}

	if (targetList.empty()) {
		shouldRot = false;
		return;
	}

	std::sort(targetList.begin(), targetList.end(), KillAura::sortByDist);

	int oldSlot = localPlayer->playerInventory->selectedSlot;
	if (autoWeaponMode != 0)
		localPlayer->playerInventory->selectedSlot = getBestWeaponSlot(targetList[0]);

	Vec3<float> aimPos = targetList[0]->getEyePos();
	aimPos.y = targetList[0]->aabbShape->aabb.getCenter().y;
	rotationAngle = localPlayer->getEyePos().CalcAngle(aimPos);
	shouldRot = true;

	if (oTick >= interval) {
		if (hitType == 1) {
			for (auto& target : targetList)
				for (int i = 0; i < multiplier; ++i)
					Attack(target);
			if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
				hitResult->type = HitResultType::ENTITY;
				hitResult->rayDirection = targetList[0]->getPos();
				hitResult->endPos = targetList[0]->getPos();
			}
		}
		else {
			for (int i = 0; i < multiplier; ++i)
				Attack(targetList[0]);
			if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
				hitResult->type = HitResultType::ENTITY;
				hitResult->rayDirection = targetList[0]->getPos();
				hitResult->endPos = targetList[0]->getPos();
			}
		}
		oTick = 0;
	}
	else {
		oTick++;
	}
	if (autoWeaponMode == 2)
		localPlayer->playerInventory->selectedSlot = oldSlot;
}

void KillAura::onUpdateRotation(LocalPlayer* localPlayer) {
	if (!shouldRot || rotMode == 0) return;
	ActorRotationComponent* rotation = localPlayer->rotation;
	MobBodyRotationComponent* bodyRot = localPlayer->getMobBodyRotationComponent();
	ActorHeadRotationComponent* headRot = localPlayer->getActorHeadRotationComponent();
	if (rotMode == 3) {
		Vec3<float> aimPos = targetList[0]->getEyePos();
		aimPos.y = targetList[0]->aabbShape->aabb.getCenter().y;
		rotationAngle = localPlayer->getEyePos().CalcAngle(aimPos);
	}
	if (rotMode == 2 || rotMode == 3) {
		rotation->presentRot.y = rotationAngle.y;
		rotation->presentRot.x = rotationAngle.x;
		bodyRot->bodyYaw = rotationAngle.y;
		headRot->headYaw = rotationAngle.y;
	}
}

void KillAura::onSendPacket(Packet* packet) {
	if (!shouldRot || rotMode == 0 || packet->getId() != PacketID::PlayerAuthInput) return;

	PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
	paip->rotation.y = rotationAngle.y;
	paip->headYaw = rotationAngle.x;
}
