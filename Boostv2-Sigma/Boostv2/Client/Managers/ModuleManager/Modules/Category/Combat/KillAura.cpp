#include "KillAura.h"
#include "../../Client/Client.h"
#include "../../../../../../Utils/FileUtil.h"
int pageset = 0;
KillAura::KillAura() : Module("KillAura", "Attacks entities around you", Category::COMBAT) {
	registerSetting(new PageSetting("Page", "Module Page", {
		 "Target", "Delays", "Rots", "Switch", "Render"
		}, &pageset));

	this->modulePagePtr = &pageset;
	registerSetting(new SliderSetting<float>("Range", "Range in which targets will be hit", &range, 5.f, 3.f, 150.f, 0));
	registerSetting(new SliderSetting<float>("WallRange", "Range in which targets will be hit through walls", &wallRange, 0.f, 0.f, 150.f, 0));
	registerSetting(new SliderSetting<float>("predict", "dd", &predictvalue, 0.f, 0.f, 15.f, 2));
	registerSetting(new SliderSetting<float>("test", "dd", &test, 0.f, 0.f, 15.f, 2));
	registerSetting(new SliderSetting<float>("headspeed", "dd", &headspeed, 30.f, 30.f, 360.f, 2));
	registerSetting(new SliderSetting<int>("Interval", "Attack delay (Tick)", &interval, 1, 0, 20, 1));
	registerSetting(new SliderSetting<int>("Multiplier", "numbers of attack per tick", &multiplier, 1, 1, 5, 1));
	registerSetting(new EnumSetting("Rotations", "Rotates to the targets", { "None", "Normal", "Strafe", "Predict" }, &rotMode, 1, 2));
	registerSetting(new BoolSetting("Randomize", "NULL", &randomize, false, 1));
	registerSetting(new EnumSetting("HitType", "NULL", { "Single", "Multi" }, &hitType, 0));
	registerSetting(new SliderSetting<int>("HitChance", "NULL", &hitChance, 100, 0, 100, 0));
	registerSetting(new EnumSetting("Weapon", "Auto switch to best weapon", { "None", "Switch", "Spoof" }, &autoWeaponMode, 0, 3));
	registerSetting(new BoolSetting("Target Visualize", "NULL", &targetVisualize, targetVisualize, 4));
	registerSetting(new ColorSetting("TV Color", "NULL", &visualizeColor, visualizeColor, 4));
	registerSetting(new BoolSetting("Mobs", "Attack Mobs", &includeMobs, false, 0));
	//registerSetting(new BoolSetting("HurtTimeCheck", "Don't attack if target is on hurt", &hurtTimeCheck, false));
}
bool SpoofSwitch(int slot) {
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

std::string KillAura::getModeText() {
	switch (hitType) {
	case 0: {
		return "Single";
		break;
	}
	case 1: {
		return "Multi";
		break;
	}
	}
	return "NULL";
}

void KillAura::onDisable() {
	targetList.clear();
	oTick = INT_MAX;
	shouldRot = false;
}

void KillAura::onEnable() {
}

bool KillAura::sortByDist(Actor* a1, Actor* a2) {
	Vec3<float> lpPos = Game.getLocalPlayer()->getPos();
	return ((a1->getPos().dist(lpPos)) < (a2->getPos().dist(lpPos)));
}

int KillAura::getBestWeaponSlot(Actor* target) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	Container* inventory = localPlayer->playerInventory->container;
	float damage = 0.f;
	int slot = localPlayer->playerInventory->selectedSlot;
	for (int i = 0; i < 9; i++) {
		localPlayer->playerInventory->selectedSlot = i;
		float currentDamage = localPlayer->calculateAttackDamage(target);
		if (currentDamage > damage) {
			damage = currentDamage;
			slot = i;
		}
	}

	return slot;
}

void KillAura::Attack(Actor* target) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	GameMode* gm = localPlayer->gameMode;

	int randomNumber = (int)(rand() % 100);
	if (randomNumber < hitChance) {
		gm->attack(target);
	}

	localPlayer->swing();
}

void KillAura::onNormalTick(LocalPlayer* localPlayer) {
	Level* level = localPlayer->level;
	BlockSource* region = Game.clientInstance->getRegion();

	targetList.clear();

	for (auto& entity : level->getRuntimeActorList()) {
		if (TargetUtil::isFriendValid(entity, includeMobs)) {
			float rangeCheck = range;
			if (region->getSeenPercent(localPlayer->getEyePos(), entity->aabbShape->aabb) == 0.0f)
				rangeCheck = wallRange;

			if (WorldUtil::distanceToEntity(localPlayer->getPos(), entity) <= rangeCheck)
				targetList.push_back(entity);
		}
	}

	if (targetList.empty()) {
		shouldRot = false;
		return;
	}

	std::sort(targetList.begin(), targetList.end(), KillAura::sortByDist);

	int oldSlot = localPlayer->playerInventory->selectedSlot;
	if (autoWeaponMode == 2) {
		SpoofSwitch(getBestWeaponSlot(targetList[0]));
		Game.getLocalPlayer()->playerInventory->selectedSlot = getBestWeaponSlot(targetList[0]);
	}
	else if (autoWeaponMode == 1)
		Game.getLocalPlayer()->playerInventory->selectedSlot = getBestWeaponSlot(targetList[0]);

	//rotAngle = localPlayer->getEyePos().CalcAngle(targetList[0]->getPos());
	//rotAngle = localPlayer->getEyePos().CalcAngle(targetList[0]->aabbShape->aabb.getCenter());

	Vec3<float> aimPos = targetList[0]->getEyePos();
	aimPos.y = targetList[0]->aabbShape->aabb.getCenter().y;
	rotAngle = localPlayer->getEyePos().CalcAngle(aimPos);
	rotAngle5 = localPlayer->getEyePos().CalcAngle(aimPos);
	Vec2<float> ange = localPlayer->getPos().CalcAngle(targetList[0]->getPos()).normAngles();

	

	shouldRot = true;

	if (oTick >= interval) {
		if (hitType == 1) {
			for (auto& target : targetList) {
				for (int j = 0; j < multiplier; ++j) {

					if (targetList[0]->rotation->presentRot.y >= 90 && targetList[0]->rotation->presentRot.y <= 93) {

						Attack(target);


					}
					else {
						Attack(target);
					}


					//Attack(target);
				}
			}
		}
		else {
			for (int j = 0; j < multiplier; ++j) {
				// Attack(targetList[0]);
				if (targetList[0]->rotation->presentRot.y >= 90 && targetList[0]->rotation->presentRot.y <= 93) {
					Attack(targetList[0]);
				}
				else {
					Attack(targetList[0]);
				}

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

	if (!shouldRot)
		return;

	ActorRotationComponent* rotation = localPlayer->rotation;
	ActorHeadRotationComponent* headRot = localPlayer->getActorHeadRotationComponent();
	MobBodyRotationComponent* bodyRot = localPlayer->getMobBodyRotationComponent();

	switch (rotMode) {
	case 0: {
		return;
		break;
	}
	case 1: {
		rotation->presentRot.x = rotAngle5.x;
		rotation->presentRot.y = rotAngle5.y;
		headRot->headYaw = rotAngle5.y;
		bodyRot->bodyYaw = rotAngle5.y;
		break;
	}


	case 2: {
		rotation->presentRot = rotAngle5;
		break;
	}
	case 3: { // PREDICT STRAFE
		if (!targetList.empty()) {
			float distanceXZ1 = localPlayer->getPos().dist(Vec3<float>(targetList[0]->getPos().x, localPlayer->getPos().y, targetList[0]->getPos().z));
			Actor* target = targetList[0];
			float targetYaw = target->rotation->presentRot.y;
			float predictYaw = targetYaw + (headspeed - 90.0f); // 180 graus atrás
			if (distanceXZ1 < predictvalue) {

				// Converter graus para radianos
				float predictRad = predictYaw * (PI / 180.0f);

				// Calcular a posição 3 blocos atrás na direção prevista
				Vec3<float> targetPos = target->getPos();
				Vec3<float> backwardOffset(-cos(predictRad) * test, 0, -sin(predictRad) * test);
				Vec3<float> backwardPos = Vec3<float>(targetPos.x + backwardOffset.x, targetPos.y + backwardOffset.y, targetPos.z + backwardOffset.z);


				rotAngle = localPlayer->getPos().CalcAngle(backwardPos).normAngles();
				Vec3<float> aimPos = targetList[0]->getEyePos();
				rotAngle2 = localPlayer->getEyePos().CalcAngle(aimPos).normAngles();

				rotation->presentRot.y = rotAngle.y;
				rotation->presentRot.x = rotAngle.x;

			}
			else
			{
				Vec3<float> aimPos = targetList[0]->getEyePos();
				rotAngle = localPlayer->getPos().CalcAngle(aimPos).normAngles();
				rotAngle2 = localPlayer->getEyePos().CalcAngle(aimPos).normAngles();
				rotation->presentRot.y = rotAngle.y;
				rotation->presentRot.x = rotAngle.x;

			}
		}
		break;
	}



	}

}
void KillAura::onSendPacket(Packet* packet) {
	if (rotMode == 0)
		return;

	if (!shouldRot)
		return;

	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
		paip->rotation = rotAngle;
		paip->headYaw = rotAngle.y;
	}
}

void KillAura::onD2DRender() {
	if (targetVisualize) {
		if (!targetList.empty() && targetList[0] != nullptr) {

			static unsigned int anim = 0;
			anim++;
			float height = targetList[0]->getAABB().upper.y - targetList[0]->getAABB().lower.y;
			const float coolAnim = (height / 2.f) + (height / 2.f) * sin(((float)anim / 60.f) * PI * 0.8f);

			Vec3<float> tgPos = targetList[0]->stateVector->pos;
			if (targetList[0]->getActorTypeComponent()->id == 319) tgPos.y -= 1.6f;

			std::vector<Vec2<float>> pointsList;
			for (int i = 0; i < 360; i += 10) {
				float calcYaw = (i + 90) * (PI / 180);
				float x = cos(calcYaw) * 0.7f;
				float z = sin(calcYaw) * 0.7f;
				static Vec2<float> pointsVec2;
				if (Game.getClientInstance()->WorldToScreen(tgPos.add2(x, coolAnim, z), pointsVec2)) {
					pointsList.push_back(pointsVec2);
				}
			}
			for (int i = 0; i < pointsList.size(); i++) {
				int next = i + 1;
				if (next == pointsList.size()) next = 0;
				D2D::drawLine(pointsList[i], pointsList[next], visualizeColor, 2.f);
			}
		}
	}
}