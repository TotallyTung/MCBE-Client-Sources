// Created by Tony on 2024-10-10 10:01:54

#include "Regen.h"
#include "../../../Client.h"
Regen::Regen() : Module("Regen", "NULL", Category::PLAYER) {
	addSlider<float>("Range", "NULL", ValueType::FLOAT_T, &range, 3.f, 10.f);
	addBoolCheck("Silent Switch", "Auto switch to the best pickaxe in hotbar silent", &silenSwitch);
	addBoolCheck("Silent Back", "Switch back to old hotbar after silent switch", &silentBack);
	addEnumSetting("Break Mode", "NULL", { "Auto", "Keybind" }, &breakMode);
	addKeybindSetting("Break keybind", "NULL", &breakKeybind);
}

void Regen::setBreakPos(const Vec3<int>& bPos, uint8_t f) {
	GameMode* gm = mc.getGameMode();
	if (gm != nullptr) breakProgess = 0.f;
	this->breakPos = bPos;
	this->face = f;
}

void Regen::Reset() {
	setBreakPos(Vec3<int>(0, 0, 0), -1);
}

std::pair<int, float> Regen::getBestPickaxeSlot(Block* block) {
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
	return std::make_pair(bestSlot, destroyRate);
}
bool isAirBlock(Vec3<int> bp) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	BlockSource* bs = localPlayer->dimension->blockSource;
	const Block* block = bs->getBlock(bp);
	if (block->blockLegacy == nullptr) return false;
	return block->blockLegacy->isAir();
}
std::vector<Vec3<int>> getBlocksWithinDistance(Vec3<float> pos, float maxDistance) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	BlockSource* bs = localPlayer->dimension->blockSource;

	std::vector<Vec3<int>> blocks;

	for (int x = pos.x - maxDistance; x < pos.x + maxDistance; x++) {
		for (int y = pos.y - maxDistance; y < pos.y + maxDistance; y++) {
			for (int z = pos.z - maxDistance; z < pos.z + maxDistance; z++) {
				Vec3<int> blockPos = Vec3<int>(x, y, z);
				const Block* block = bs->getBlock(blockPos);
				if (block->blockLegacy->blockId == 56) {
					blocks.push_back(blockPos);
				}
			}
		}
	}

	// Sort the blocks by distance to the player (player->getPosition())
	std::sort(blocks.begin(), blocks.end(), [pos](Vec3<int> a, Vec3<int> b) {
		return pos.distanceTo(a) < pos.distanceTo(b);
		});

	return blocks;
}
int getBlockBreakFace(Vec3<int> bp) {
	if (isAirBlock(bp + Vec3<int>(0, -1, 0))) return 1;
	if (isAirBlock(bp + Vec3<int>(0, 0, 1))) return 2;
	if (isAirBlock(bp + Vec3<int>(0, 0, -1))) return 3;
	if (isAirBlock(bp + Vec3<int>(1, 0, 0))) return 4;
	if (isAirBlock(bp + Vec3<int>(-1, 0, 0))) return 5;
	if (isAirBlock(bp + Vec3<int>(0, 1, 0))) return 0;

	return -1;
}

void Regen::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = (LocalPlayer*)actor;
	GameMode* gm = localPlayer->getGameMode();
	if (silenSwitch && shouldSwitchBack && lastSlot != -1) {
		MobEquipmentPacket pk(localPlayer->getRuntimeID(), localPlayer->getPlayerInventory()->inventory->getItemStack(lastSlot), lastSlot, lastSlot);
		mc.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
	}
	BlockSource* bs = localPlayer->dimension->blockSource;
	shouldSwitchBack = false;
	std::vector<Vec3<int>> blocks = getBlocksWithinDistance(*localPlayer->getPosition(), range);

	// Count the number of redstone ores within range
	int redstoneOreCount = 0;
	for (auto& b : blocks) {
		if (localPlayer->dimension->blockSource->getBlock(b)->blockLegacy->blockId == 56) {
			redstoneOreCount++;
		}
	}

	for (auto& b : blocks) {
		breakPos = b;
		if (breakPos != Vec3<int>(0, 0, 0) && getBlockBreakFace(breakPos) == -1) {
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

			// Calculate break progress based on the number of redstone ores being broken
			float breakProgressIncrement = 1.f / redstoneOreCount;
			if (breakProgess < 1.f) {
				if (silenSwitch) breakProgess += breakProgressIncrement * bestSlot.second;
				else breakProgess += breakProgressIncrement * (gm->getDestroyRate(block));
				if (breakProgess > 1.f) breakProgess = 1.f;
				// Break the current block only if the break progress is complete
				if (breakProgess >= 1.f) {
					breakBlock(localPlayer, gm, bestSlot);
				}
			}
		}
	}
}

void Regen::breakBlock(LocalPlayer* localPlayer, GameMode* gm, std::pair<int, float>& bestSlot) {
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
	gm->destroyBlock(breakPos, face);
	if (silenSwitch) {
		mc.getLocalPlayer()->getPlayerInventory()->selectedSlot = oldSlot;
		if (silentBack) {
			shouldSwitchBack = true;
			lastSlot = localPlayer->getPlayerInventory()->selectedSlot;
		}
	}
	this->Reset();
}

void Regen::onRender(MinecraftUIRenderContext* ctx) {
	GameMode* gm = mc.getGameMode();
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (gm == nullptr || localPlayer == nullptr || !mc.getClientInstance()->minecraftGame->canUseKeys) return;

	ImGuiIO& io = ImGui::GetIO();
	static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");

	float destroyProgress = breakProgess;
	if (destroyProgress > 1.f) destroyProgress = 1.f;

	UIColor fillColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 40);
	UIColor lineColor(255 - int(destroyProgress * 255.f), int(destroyProgress * 255.f), 0, 225);
	if (destroyProgress > 0.f) {
		AABB aabb = localPlayer->makeAABB(&breakPos);
		RenderUtils::drawBox(aabb, fillColor, lineColor, 0.3f, true, false);
	}
}

void Regen::onImGuiRender(ImDrawList* drawlist) {
}