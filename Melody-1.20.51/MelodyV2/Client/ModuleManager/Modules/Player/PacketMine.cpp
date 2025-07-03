// Created by Tony on 2024-10-10 10:01:54

#include "PacketMine.h"
#include "../../../Client.h"
bool countinue = false;
float Cspeed = 0.1f;
PacketMine::PacketMine() : Module("PacketMine", "NULL", Category::PLAYER) {
	addSlider<float>("Range", "NULL", ValueType::FLOAT_T, &range, 3.f, 10.f);
	addBoolCheck("Countinue", "Still Mines that blocks", &countinue);
	addSlider<float>("Countinue Speed", "NULL", ValueType::FLOAT_T, &Cspeed, 0.01f, 1.f);
	addBoolCheck("Silent Switch", "Auto switch to the best pickaxe in hotbar silent", &silenSwitch);
	addBoolCheck("Silent Back", "Switch back to old hotbar after silent switch", &silentBack);
	addEnumSetting("Break Mode", "NULL", { "Auto", "Keybind" }, &breakMode);
	addKeybindSetting("Break keybind", "NULL", &breakKeybind);
}
Vec3<float> possrot;
void PacketMine::setBreakPos(const Vec3<int>& bPos, uint8_t f) {
	GameMode* gm = mc.getGameMode();
	if (gm != nullptr) gm->destroyProgress = 0.f;
	this->breakPos = bPos;
	this->face = f;
}

void PacketMine::Reset() {
	possrot = Vec3<float>(0, 0, 0);
	setBreakPos(Vec3<int>(0, 0, 0), -1);
}
float lastDestroyRate = 0.f;
std::pair<int, float> PacketMine::getBestPickaxeSlot(Block* block) {
	GameMode* gm = mc.getGameMode();
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	int oldSlot = plrInv->selectedSlot;
	int bestSlot = plrInv->selectedSlot;
	float destroyRate = gm->getDestroyRate(block);
	for (int i = 0; i < 9; i++) {
		plrInv->selectedSlot = i;
		float value = gm->getDestroyRate(block);
		if (value > destroyRate) {
			destroyRate = value;
			bestSlot = i;
		}
	}

	plrInv->selectedSlot = oldSlot;
	if (block->blockLegacy->blockId != 0) {
		lastDestroyRate = destroyRate;
	}
	else {
		if (countinue) {
			destroyRate = lastDestroyRate;
		}
	}
	return std::make_pair(bestSlot, destroyRate);
}


void PacketMine::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = (LocalPlayer*)actor;
	GameMode* gm = localPlayer->getGameMode();
	if (silenSwitch && shouldSwitchBack && lastSlot != -1) {
		MobEquipmentPacket pk(localPlayer->getRuntimeID(), localPlayer->getPlayerInventory()->inventory->getItemStack(lastSlot), lastSlot, lastSlot);
		mc.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
	}
	isbeingmine = false;
	shouldSwitchBack = false;
	if (breakPos != Vec3<int>(0, 0, 0)) {
		if (breakPos.dist(*localPlayer->getPosition()) > range) {
			this->Reset();
			return;
		}
		Block* block = localPlayer->dimension->blockSource->getBlock(breakPos);
		if (block == nullptr || block->blockLegacy == nullptr) {
			this->Reset();
			return;
		}
		std::pair<int, float> bestSlot = getBestPickaxeSlot(block);
		if (gm->destroyProgress < 1.f) {
			isbeingmine = true;
			setIsMining(true);
			if (silenSwitch) gm->destroyProgress += bestSlot.second;
			else gm->destroyProgress += gm->getDestroyRate(block);
			if (gm->destroyProgress > 1.f) gm->destroyProgress = 1.f;
		}
		else {
			if (breakMode == 1) {
				if (breakKeybind == 0x0) return;
				if (!mc.isKeyDown(breakKeybind)) return;
			}
			int oldSlot = mc.getLocalPlayer()->getPlayerInventory()->selectedSlot;
			if (silenSwitch) {
				mc.getLocalPlayer()->getPlayerInventory()->selectedSlot = bestSlot.first;
				MobEquipmentPacket pk(localPlayer->getRuntimeID(), localPlayer->getPlayerInventory()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
				mc.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
			}

			bool oldIsDestroying = mc.getLocalPlayer()->isDestroying;
			mc.getLocalPlayer()->isDestroying = true;
			possrot = breakPos.toFloat();
			setIsMining(true); // Set mining flag to true when mining starts
			gm->destroyBlock(breakPos, face);

			if (silenSwitch) {
				mc.getLocalPlayer()->getPlayerInventory()->selectedSlot = oldSlot;
				if (silentBack) {
					shouldSwitchBack = true;
					lastSlot = localPlayer->getPlayerInventory()->selectedSlot;
				}
			}
			mc.getLocalPlayer()->isDestroying = oldIsDestroying;
			if (countinue) {
				gm->destroyProgress = Cspeed;
			}
			else {
				setIsMining(false); // Set mining flag to false when mining stops
				this->Reset();
			}
		}
	}
	isbeingmine = false;
}
void PacketMine::onRender(MinecraftUIRenderContext* ctx) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (gm == nullptr || localPlayer == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;

	ImGuiIO& io = ImGui::GetIO();
	static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");

	float destroyProgress = gm->destroyProgress;
	if (destroyProgress > 1.f) destroyProgress = 1.f;

	UIColor fillColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 40);
	UIColor lineColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 225);
	if (destroyProgress > 0.f) {
		AABB aabb = localPlayer->makeAABB(&breakPos);
		RenderUtils::drawBox(aabb, fillColor, lineColor, 0.3f, true, false);
	}
}
void PacketMine::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (packet->getId() == PacketID::PlayerAuthInput || packet->getId() == PacketID::MovePlayerPacket) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		auto rots = GetRotations(mc.getLocalPlayer()->stateVectorComponent->pos, possrot);
		if (mc.getLocalPlayer()->isDestroying) {
			authPacket->ticksAlive = 20;
			movePacket->tick = 20;
			movePacket->actorRuntimeID = mc.getLocalPlayer()->getRuntimeID();
			authPacket->rotation = rots;
			movePacket->rotation = rots;
			authPacket->headYaw = rots.y;
			movePacket->headYaw = rots.y;
			//char message2[256];  // Adjust the buffer size as needed
			//sprintf(message2, "id  = ", rots);
			//mc.DisplayClientMessage(message2);
		}
		 

	}
}
void PacketMine::onImGuiRender(ImDrawList* drawlist) {
}