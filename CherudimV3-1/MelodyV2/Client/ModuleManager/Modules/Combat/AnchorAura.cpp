#include "AnchorAura.h"

bool Geyser = false;
AnchorAura::AnchorAura() : Module("AnchorAura", "Automatically places Anchor above players to destroy helmets or get them stuck", Category::COMBAT) {
	addSlider<int>("Range", "NULL", ValueType::INT_T, &range, 3, 10);
	addSlider<int>("Adelay", "NULL", ValueType::INT_T, &anchordelay, 0, 10);
	addSlider<int>("Gdelay", "NULL", ValueType::INT_T, &gsdelay, 0, 10);
	addSlider<int>("Bdelay", "NULL", ValueType::INT_T, &breakdelay, 0, 10);
	addBoolCheck("Silent", "NULL", &airplace);
	addBoolCheck("JavaMode", "NULL", &Geyser);
	//this->registerIntSetting("range", &this->range, this->range, 3, 10);
	//this->registerIntSetting("anchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	//this->registerIntSetting("gsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	//this->registerIntSetting("Break delay", &this->breakdelay, this->breakdelay, 0, 10);
	//this->registerBoolSetting("Mutil", &this->airplace, this->airplace);
}
AnchorAura::~AnchorAura() {
}
std::string AnchorAura::getModName() {
	return names;

}

static std::vector<Actor*> targetLis2;

void AnchorAura::onEnable() {
	targetLis2.clear();
}

static int getAnchor() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 65264) {
				return i;
			}
		}
	}
	return slot;
}//
void getGS2() {
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 89) {
				supplies->selectedSlot = i;
				return;
			}
		}
	}
}
void getAnchor2(){
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	/*
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 89) {  // select anchor
					supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}*/
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 65264) {
				supplies->selectedSlot = i;
				return;
			}
		}
	}
}
static int getGS() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 89) {
				return i;
			}
		}
	}
	return slot;
}//

static void predictBlock(Vec3<float> pos) {
	GameMode* gm = mc.getGameMode();
	static std::vector<Vec3<float>> blocks;

	if (blocks.empty()) {
		for (int y = -2; y <= 2; y++) {
			for (int x = -5; x <= 5; x++) {
				for (int z = -5; z <= 5; z++) {
					blocks.emplace_back(Vec3<float>(x, y, z));
				}
			}
		}

		std::ranges::sort(blocks, {}, &Math::calculateDistance);
	}

	auto tryPlaceBlock = [&](const Vec3<float>& offset) {
		Vec3<float> currentBlock = (Vec3<float>(pos.floor())).add(offset);
		if (gm->tryPlaceBlock(currentBlock.toInt())) {
			return true;
		}
		return false;
		};

	for (const Vec3<float>& offset : blocks) {
		if (tryPlaceBlock(offset)) {
			return;
		}
	}
}

static void tryAnchorCool(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getAnchor();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		predictBlock(tryBuildPos.toFloat());

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
static void tryGSCool(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 66063) {

		int bestSlot = getGS();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		//predictBlock(tryBuildPos.toFloat());
		gm->buildBlock(tryBuildPos, 0, 0);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
static void tryBreak(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 66063) {

		int bestSlot = getAnchor();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		//predictBlock(tryBuildPos.toFloat());
		gm->buildBlock(tryBuildPos, 0, 0);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}

/*void charge2(Vec3<float> gsplace) {
	gsplace = gsplace.floor();

	C_Block* block = mc.getLocalPlayer()->region->getBlock(Vec3<float>(gsplace));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3<float> blok(gsplace);
		int i = 0;
			mc.getCGameMode()->buildBlock(&blok, i);  // place on face with respawn anchor
			return;
	}
	return;
}*/





void AnchorAura::onNormalTick(Actor* actor) {
	if (mc.getLocalPlayer() == nullptr)
		return;
	if (!mc.canUseMoveKeys())
		return;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	GameMode* gm = mc.getGameMode();
	BlockSource* region = localPlayer->dimension->blockSource;
	Level* level = localPlayer->getLevel();
	targetLis2.clear();

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, airplace)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) targetLis2.push_back(actor);
		}
	}
	int place = 0;

	if (!targetLis2.empty()) {
			Vec3<float> enemyLoc = *targetLis2[0]->getPosition();
			auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(enemyLoc.toFloat());
			rotAnglePlace = rotationToPlacement;
			if(Geyser)
			bottom1 = enemyLoc.add(-1, 1, 0);
			else
			bottom1 = enemyLoc.add(0, 1, 0);
			if (!hasPlacedAnchor) {
				// NOT placed anchor
				if (!takenAnchor) {
					if(airplace)
					getAnchor2();
					takenAnchor = true;
					return;
				}

				Block* block = localPlayer->dimension->blockSource->getBlock(bottom1.toInt());
				if (block->blockLegacy->blockId == 0) {
					for (auto& i : targetLis2)
						if (airplace)
							//placeBlok2(bottom1);
							gm->buildBlock(Vec3<float>(bottom1).toInt(), 0, 0);
							//gm->buildBlock(Vec3<float>(bottom1).toInt(), 0,0);
						else
							//tryAnchor(Vec3<float>(bottom1).toInt());
							
					tryAnchorCool(Vec3<float>(bottom1).toInt());
					Option = 1;
				}
				hasPlacedAnchor = true;
				//clientMessageF("Placed anchor!");
			}

			if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
				tickTimer = 0;
				DhasPlacedAnchor = true;
			}
			else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
				tickTimer++;
				return;
			}

			if (!hasCharged) {
				if (!takenGS) {
					if (airplace)
					getGS2();
					takenGS = true;
					return;
				}

				switch (Option) {
				case 1:
					Block * block2 = localPlayer->dimension->blockSource->getBlock(bottom1.toInt());
					int sb = block2->blockLegacy->blockId;
					//mc.DisplayClientMessage("%d", sb);
					if (block2->blockLegacy->blockId == 66063) {
						if (airplace)
							//placeBlok2(bottom1);
							gm->buildBlock(Vec3<float>(bottom1).toInt(), 0, 0);
						//gm->buildBlock(Vec3<float>(bottom1).toInt(), 0,0);
						else
							//tryAnchor(Vec3<float>(bottom1).toInt());

							tryGSCool(Vec3<float>(bottom1).toInt());
					}
					break;
				}

				hasCharged = true;
				takenAnchor = false;
				//clientMessageF("Charging!");
			}

			if (tickTimer >= gsdelay && !DhasCharged) {
				tickTimer = 0;
				DhasCharged = true;
			}
			else if (tickTimer < gsdelay && !DhasCharged) {
				tickTimer++;
				return;
			}

			if (!takenAnchor) {
				if (airplace)
				getAnchor2();
				takenAnchor = true;
				return;
			}

			if (!hasDetonated) {
				switch (Option) {
				case 1:
					if (airplace)
						//placeBlok2(bottom1);
						gm->buildBlock(Vec3<float>(bottom1).toInt(), 0, 0);
					//gm->buildBlock(Vec3<float>(bottom1).toInt(), 0,0);
					else
						//tryAnchor(Vec3<float>(bottom1).toInt());

						tryBreak(Vec3<float>(bottom1).toInt());

					//gm->buildBlock(&Vec3<float>(bottom1), 0);
					break;
				}
				hasDetonated = true;
			}
			//clientMessageF("Detonated!");

			if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
				hasPlacedAnchor = false;
				hasCharged = false;
				hasDetonated = false;
				DhasPlacedAnchor = false;
				DhasCharged = false;
				tickTimer = 0;
				takenAnchor = false;
				takenGS = false;
				return;
			}
			else {
				tickTimer++;
				return;
			}
		
	}
}
void AnchorAura::onRender(MinecraftUIRenderContext* renderCtx) {
}

void AnchorAura::onSendPacket(Packet* packet, bool& shouldCancel)
{
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rotation = rotAnglePlace;
		authPacket->headYaw = rotAnglePlace.y;
	}
}

void AnchorAura::onDisable() {
}