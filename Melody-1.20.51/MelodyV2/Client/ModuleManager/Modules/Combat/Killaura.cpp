// Created by Tony on 2024-10-10 10:01:54

#include "Killaura.h"
#include <cmath> // for std::atan2()
Killaura::Killaura() : Module("Killaura", "Auto attack players / mobs arround u.", Category::COMBAT) {
	addSlider<float>("Target Range", "Players/Mobs have range lower than this will be targeted", ValueType::FLOAT_T, &targetRange, 3.f, 12.f);
	addSlider<float>("Wall Range", "NULL", ValueType::FLOAT_T, &wallRange, 0.f, 12.f);
	addEnumSetting("Mode", "NULL", { "Single", "Multi" }, &Mode);
	addEnumSetting("Rotation", "NULL", { "None", "Silent" ,"Strafe"}, &rotMode);
	addEnumSetting("Switch", "NULL", { "None", "Full", "Silent" }, &switchMode);
	addBoolCheck("Attack Mob", "If u want attack mob or not", &attackMob);
	addBoolCheck("Hurttime check", "NULL", &hurttime);
	addSlider<int>("Attack delay", "NULL", ValueType::INT_T, &attackDelay, 0, 20);
	addBoolCheck("Visual Range", "NULL", &visualRange);
	addColorPicker("VR Color", "NULL", &vRColor);
	addBoolCheck("Target Visualize", "NULL", &targetVisualize);
	addColorPicker("TV Color", "NULL", &visualizeColor);
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
			float seenPercent = region->getSeenPercent(localPlayer->stateVectorComponent->pos, *target->getAABB());
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = (seenPercent > 0.f) ? targetRange : wallRange;
			if (dist < rangeCheck) targetList.push_back(target);
		}
	}

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);

		if (attackDelayTick >= attackDelay) {
			int bestSlot = getBestWeaponSlot();
			int oldSlot = plrInv->selectedSlot;
			bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

			if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
				plrInv->selectedSlot = bestSlot;

				if (switchMode == 2) {
					/*auto packet = MinecraftPackets::createPacket(PacketID::MobEquipmentPacket);
					auto packetcasted = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
					packetcasted->ActorRuntimeID = localPlayer->getRuntimeID();
					packetcasted->ContainerID = 0;
					packetcasted->ContainerID2 = 0;
					packetcasted->InventorySlot = bestSlot;
					packetcasted->InventorySlot2 = bestSlot;
					packetcasted->HotbarSlot = bestSlot;
					packetcasted->HotbarSlot2 = bestSlot;
					NetworkItemStackDescriptor itemstackdescriptor(*inv->getItemStack(bestSlot));
					packetcasted->ItemStackDescriptor = itemstackdescriptor;
					mc.getClientInstance()->loopbackPacketSender->send(packetcasted.get());*/ // <- this hehh tao lay tu nuvola tai vi no ngon hon
					MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
					mc.getLocalPlayer()->sendNetworkPacket(pk);
					mc.getClientInstance()->loopbackPacketSender->send(&pk);
				}
			}
			if (Mode = 1)
			{
				for (auto& target : targetList) {
					if (!(hurttime && target->hurtTime > 0)) {
						InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), target->stateVectorComponent->pos.sub(Vec3<int>(0.f,0.2f,0.f)));

						mc.getClientInstance()->loopbackPacketSender->send(&inter);
						gm->attack(target);
						localPlayer->swing();
					}
				}
			}
			if (Mode = 0)
			{
				if (!(hurttime && targetList[0]->hurtTime > 0)) {
					InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), targetList[0]->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

					mc.getClientInstance()->loopbackPacketSender->send(&inter);
					gm->attack(targetList[0]);
					localPlayer->swing();
				}
			}
			

			rotAngle = GetRotations(localPlayer->stateVectorComponent->pos, targetList[0]->stateVectorComponent->pos.sub(Vec3<float>(0.f, 0.9f, 0.f)));

			if (rotMode == 2) {
				//auto haah = std::to_string(inv->getItemStack(oldSlot)->getItemPtr()->itemId);
				localPlayer->rotationComponent->rotation = rotAngle;
				localPlayer->rotationComponent->Set(rotAngle);
				localPlayer->rotationComponent->oldRotation = *localPlayer->getRotationPrev();
				//char message2[256];  // Adjust the buffer size as needed
				//sprintf(message2, "id  = ", haah);
				//mc.DisplayClientMessage(haah.c_str());

			}

			if (shouldSwitch && switchMode == 2) {
				plrInv->selectedSlot = oldSlot;
			}

			attackDelayTick = 0;

		}
		else {
			attackDelayTick++;
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
	if (mc.getClientInstance()->getLevelRenderer() == nullptr) return;
	if (mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;

	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (visualRange) {
		Vec3<float> lpPos = localPlayer->stateVectorComponent->pos;
		if (mc.cameraPerspectiveMode == 0) lpPos = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
		std::vector<Vec2<float>> pointsList;
		for (int i = 0; i < 360; i += 4) {
			float calcYaw = (i + 90) * (PI / 180);
			float x = cos(calcYaw) * targetRange;
			float z = sin(calcYaw) * targetRange;
			static Vec2<float> pointsVec2;
			if (ImGuiUtils::worldToScreen(lpPos.add(x, -1.6f, z), pointsVec2)) {
				pointsList.push_back(pointsVec2);
			}
		}
		for (int i = 0; i < pointsList.size(); i++) {
			int next = i + 1;
			if (next >= pointsList.size()) next = 0;
			d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), vRColor.toImColor(), 2.f);
		}
	}
	ImGuiIO& io = ImGui::GetIO();
	if (targetVisualize) {
		if (!targetList.empty() && targetList[0] != nullptr) {

			static unsigned int anim = 0;
			anim++;
			float height = targetList[0]->aabbComponent->aabb.upper.y - targetList[0]->aabbComponent->aabb.lower.y;
			const float coolAnim = (height / 2.f) + (height / 2.f) * sin(((float)anim / 60.f) * PI * 0.8f);

			Vec3<float> tgPos = targetList[0]->stateVectorComponent->pos;
			if (targetList[0]->isPlayer()) tgPos.y -= 1.6f;

			std::vector<Vec2<float>> pointsList;
			for (int i = 0; i < 360; i += 10) {
				float calcYaw = (i + 90) * (PI / 180);
				float x = cos(calcYaw) * 0.7f;
				float z = sin(calcYaw) * 0.7f;
				static Vec2<float> pointsVec2;
				if (ImGuiUtils::worldToScreen(tgPos.add(x, coolAnim, z), pointsVec2)) {
					pointsList.push_back(pointsVec2);
				}
			}
			for (int i = 0; i < pointsList.size(); i++) {
				int next = i + 1;
				if (next == pointsList.size()) next = 0;
				d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), visualizeColor.toImColor(), 2.f);
			}
		}
	}
}