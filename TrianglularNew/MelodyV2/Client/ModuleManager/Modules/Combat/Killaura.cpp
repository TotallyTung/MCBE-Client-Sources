#include "Killaura.h"
#include <cmath> // for std::atan2()
#include "../Exploit/AntiDesync.h"
#include "../../../Client.h"
int hitattempts = 0;
bool swing1;
bool packets;
int packetmultiplier = 0;
Killaura::Killaura() : Module("Killaura", "Auto attack players / mobs arround u.", Category::COMBAT) {
	slider<float>("Max Strafe", "NULL", ValueType::FLOAT_T, &MaxSrange, 0.f, 20.f);
	slider<float>("Min Strafe", "NULL", ValueType::FLOAT_T, &MinSrange, 0.f, 20.f);
	slider<float>("Target Range", "Players/Mobs have range lower than this will be targeted", ValueType::FLOAT_T, &targetRange, 3.f, 20.f);
	slider<float>("Attack Range", "NULL", ValueType::FLOAT_T, &ARange, 0.f, 10.f);
	enumsetting("Mode", "NULL", { "Single", "Multi" }, &Mode);
	enumsetting("Rotation", "NULL", { "None", "Silent" ,"Strafe" }, &rotMode);
	enumsetting("Switch", "NULL", { "None", "Full", "Silent" }, &switchMode);
	boolsetting("Attack Mob", "If u want attack mob or not", &attackMob);
	boolsetting("Hurttime check", "NULL", &hurttime);
	slider<int>("Attack delay", "NULL", ValueType::INT_T, &attackDelay, 0, 20);
	slider<int>("Hit Attempts", "NULL", ValueType::INT_T, &hitattempts, -1, 20);
	boolsetting("Swing", "NULL", &swing1);
	boolsetting("Packets", "NULL", &packets);
	slider<int>("Packet Multiplier", "NULL", ValueType::INT_T, &packetmultiplier, 0, 20);
}

int Killaura::getBestWeaponSlot() {
	auto localPlayer = mc.getLocalPlayer();
	if (!localPlayer)
		return -1;

	auto plrInv = localPlayer->getPlayerInventory();
	if (!plrInv)
		return -1;

	auto inv = plrInv->inventory;
	if (!inv)
		return -1;

	float damage = 0.f;
	int slot = plrInv->selectedSlot;

	for (int i = 0; i < 9; i++) {
		auto itemStack = inv->getItemStack(i);
		if (itemStack && itemStack->isValid()) { // or you can do itemStack->getItemPtr()->hasRecipeTag("minecraft:is_sword") for sword find only 
			/*
			* here some step that you can get pick and things
			* item->getItemPtr()->hasRecipeTag("minecraft:digger") <- this is how you get like a things that can digs like shovel and picks and axe maybe
			* if (item->hasRecipeTag("minecraft:is_sword"))
			* item->hasRecipeTag("minecraft:is_pickaxe") <- get pickaxe
			* item->hasRecipeTag("minecraft:is_axe") <- get axe
			* item->hasRecipeTag("minecraft:is_shovel") <- get some shovel
			* item->hasRecipeTag("minecraft:is_hoe")<- get hoe like a riel bitch
			* item->hasRecipeTag("minecraft:is_food") <- get food
			*/
			float currentDamage = itemStack->getItemPtr()->getAttackDamage() + (1.25f * itemStack->getEnchantLevel(EnchantID::sharpness));
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = i;
			}
		}
	}
	return slot;
}
# define M_PI 3.14159265358979323846 /* pi */

Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
	Vec3<float> delta = targetPos.sub(playerEyePos);
	float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
	float pitch = atan2(delta.y, std::sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
	return { -pitch, yaw - 90 };
}
void Killaura::onNormalTick(Actor* actor) {
	static AntiDesync* AntiDesyncMod = (AntiDesync*)client->moduleMgr->getModule("AntiDesync");
	if (AntiDesyncMod->isEnabled()) {
		if (mc.getLocalPlayer()->getItemUseDuration() > 0) return;
	}
	auto localPlayer = mc.getLocalPlayer();
	auto plrInv = localPlayer->getPlayerInventory();
	auto inv = plrInv->inventory;
	auto gm = mc.getGameMode();
	auto region = localPlayer->dimension->blockSource;
	auto level = localPlayer->getLevel();

	targetList.clear();
	if (!level) return;

	for (auto& target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, attackMob)) {
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = targetRange;
			if (dist < rangeCheck)
				targetList.push_back(target);
		}
	}

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);
		TargetDis = targetList[0]->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
		isInRange = true;
		if (rotMode == 2) {
			localPlayer->rotationComponent->Set(rotAngle);
			localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
			localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
		}
		if (attackDelayTick >= attackDelay) {
			int bestSlot = getBestWeaponSlot();
			int oldSlot = plrInv->selectedSlot;
			bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

			if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
				plrInv->selectedSlot = bestSlot;

				if (switchMode == 2) {
					MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
					mc.getLocalPlayer()->sendNetworkPacket(pk);
					mc.getClientInstance()->loopbackPacketSender->send(&pk);
				}
			}
			if (Mode == 1)
			{
				for (auto& target : targetList) {
					if (target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) < ARange) {
						if (!(hurttime && target->hurtTime > 0)) {
							InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), target->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));
							if (packets) {
								for (auto i = 0; i < packetmultiplier; i++) {
									mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
								}
							}
							for (auto i = 0; i < hitattempts; i++) {
								gm->attack(target);
							}
							if (swing1) {
								localPlayer->swing();
							}
							rotAngle = GetRotations(localPlayer->stateVectorComponent->pos, targetList[0]->stateVectorComponent->pos.sub(Vec3<float>(0.f, 0.9f, 0.f)));
							if (rotMode == 2) {
								localPlayer->rotationComponent->Set(rotAngle);
								localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
								localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
							}
						}
					}
				}
			}
			if (Mode == 0)
			{
				for (auto& target : targetList) {
					if (target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) < ARange) {
						if (!(hurttime && target->hurtTime > 0)) {
							InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), targetList[0]->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));
							if (packets) {
								for (auto i = 0; i < packetmultiplier; i++) {
									mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
								}
							}
							for (auto i = 0; i < hitattempts; i++) {
								gm->attack(targetList[0]);
							}
							if (swing1) {
								localPlayer->swing();
							}
							rotAngle = GetRotations(localPlayer->stateVectorComponent->pos, targetList[0]->stateVectorComponent->pos.sub(Vec3<float>(0.f, 0.9f, 0.f)));
							if (rotMode == 2) {
								localPlayer->rotationComponent->Set(rotAngle);
								localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
								localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
							}
						}
					}
				}
			}
			if (shouldSwitch && switchMode == 2) {
				plrInv->selectedSlot = oldSlot;
			}
			attackDelayTick = 0;
			if (rotMode == 2) {
				localPlayer->rotationComponent->Set(rotAngle);
				localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
				localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
			}
		}
		else {
			attackDelayTick++;
			if (rotMode == 2) {
				localPlayer->rotationComponent->Set(rotAngle);
				localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
				localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
			}
		}
	}
	else {
		isInRange = false;
		targetList.clear();
		TargetDis = 0;
		if (rotMode == 2) {
			localPlayer->rotationComponent->Set(rotAngle);
			localPlayer->rotationComponent->SetHeadYaw(rotAngle.y);
			localPlayer->rotationComponent->SetHeadPitch(rotAngle.x);
		}
	}
}

void Killaura::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}

	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->rotation = rotAngle;
		movepacket->headYaw = rotAngle.y;
	}
}

void Killaura::onImGuiRender(ImDrawList* d) {

}