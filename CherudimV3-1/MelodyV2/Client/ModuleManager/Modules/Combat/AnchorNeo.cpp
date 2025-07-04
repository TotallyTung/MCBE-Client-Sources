#include "AnchorNeo.h"
bool geyser2 = false;
AnchorNeo::AnchorNeo() : Module("AnchorNeo", "NeoAura", Category::COMBAT) {
	addSlider<int>("Range", "NULL", ValueType::INT_T, &Neorange, 3, 10);
	addBoolCheck("Self", "NULL", &silent);
	addBoolCheck("JavaMode", "NULL", &geyser2);
}

AnchorNeo::~AnchorNeo() {
}
std::string AnchorNeo::getModName() {
	return names;
}

static std::vector<Actor*> targetList;

void AnchorNeo::onEnable() {
}

static int getid(Vec3<float> pos) {
	int id = 0;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	Block* block = localPlayer->dimension->blockSource->getBlock(pos.toInt());

	id = block->blockLegacy->blockId;
	return id;
}
static void PredictBlok(Vec3<float> pos) {
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
static int getAnchor4() {
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


static int getGS4() {
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
static void tryAc(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getAnchor4();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		PredictBlok(tryBuildPos.toFloat());
		//gm->buildBlock(tryBuildPos, 0, 0);
		//gm->buildBlock(tryBuildPos, 0, 0);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
static void tryGS(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 66063) {

		int bestSlot = getGS4();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		PredictBlok(tryBuildPos.toFloat());
		//gm->buildBlock(tryBuildPos, 0, 0);
		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}





void AnchorNeo::onNormalTick(Actor* actor) {
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
	targetList.clear();

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, silent)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) targetList.push_back(actor);
		}
	}
	int place = 0;

	if (!targetList.empty()) {
		Vec3<float> enemyLoc = targetList[0]->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
		std::vector<Vec3<float>> positions = {
	{ enemyLoc.x + 1, enemyLoc.y, enemyLoc.z },
	{ enemyLoc.x - 1, enemyLoc.y, enemyLoc.z },
	{ enemyLoc.x, enemyLoc.y, enemyLoc.z + 1 },
	{ enemyLoc.x, enemyLoc.y, enemyLoc.z - 1 },
	{ enemyLoc.x, enemyLoc.y + 2, enemyLoc.z }
		};
		for (auto& pos : positions) {
			Block* block = localPlayer->dimension->blockSource->getBlock(pos.toInt());
			if (!block || !block->blockLegacy)
				continue;
			if (block->blockLegacy->blockId != 0) {
				if (block->blockLegacy->blockId != 66063) {
					continue;
				}
			}
			if (geyser2)
				pos.x += -1 ;
			else
				pos.x += 0;
			auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(pos.toFloat());
			rotAnglePlace = rotationToPlacement;
			//getAnchor6();
			if (getid(pos) == 0) {
				tryAc(pos.toInt());
				shit = true;
			}
			if (shit) {
				//getGS6();
				tryGS(pos.toInt());
				shit = false;
			}
			if (!shit) {
				gm->buildBlock(pos.toInt(), 222, true);
			}
		}
	}
}

void AnchorNeo::onRender(MinecraftUIRenderContext* renderCtx)
{
}
void AnchorNeo::onSendPacket(Packet* packet, bool& shouldCancel)
{

	
}
void AnchorNeo::onDisable()
{
}