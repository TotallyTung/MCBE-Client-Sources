#include "PacketMine.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"

PacketMine::PacketMine() : IModule(0, Category::PLAYER, "UrMomNigga") {
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	Mode.addEntry("Normal", 0);
	Mode.addEntry("Continues", 1);
	registerEnumSetting("Mode", &Mode, 0);
	Continues.addEntry("Check", 0);
	Continues.addEntry("Always", 1);
	registerEnumSetting("Continues", &Continues, 0);
	registerFloatSetting("Mine Speed", &MineSpeed, MineSpeed, 0.5f, 2.f);
	registerFloatSetting("Reset Value", &ResetValue, ResetValue, 0.f, 1.f);
	registerBoolSetting("Silent Switch", &silentSwitch, silentSwitch);
	registerBoolSetting("Silent Back", &silentBack, silentBack);
	registerBoolSetting("MultiTask", &multiTask, multiTask);
//	registerBoolSetting("Strict", &strict, strict);
	registerBoolSetting("Grow Render", &GrowRender, GrowRender);
}

const char* PacketMine::getModuleName() {
	return "PacketMine";
}

const char* PacketMine::getModName() {
	switch (Mode.getSelectedValue()) {
	case 0: {
		return "Normal";
		break;
	}
	case 1: {
		return "Continues";
		break;
	}
	case 2: {
		return "Instant";
		break;
	}
	}
	return nullptr;
}

bool PacketMine::DoWeHavePickaxe(LocalPlayer* lp) {
	PlayerInventoryProxy* supplies = lp->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int i = 0; i < 9; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->isValid()) {
			if (stack->getItem()->isPickaxe()) return true;
		}
	}
	return false;
}

bool PacketMine::isHoldingPickaxe(LocalPlayer* lp) {
	ItemStack* stack = lp->getSelectedItem();
	if (!stack->isValid()) return false;
	if (stack->getItem()->isPickaxe()) return true;
	return false;
}

std::pair<int, float> PacketMine::getBestPickaxeSlot(LocalPlayer* lp, GameMode* gm, Block* block) {
	PlayerInventoryProxy* supplies = lp->getSupplies();
	Inventory* inv = supplies->inventory;

	int bestSlot = supplies->selectedHotbarSlot;
	int oldSlot = supplies->selectedHotbarSlot;
	float destroyRate = gm->getDestroyRate(block);
	for (int i = 0; i < 9; i++) {
		supplies->selectedHotbarSlot = i;
		if (gm->getDestroyRate(block) > destroyRate) {
			destroyRate = gm->getDestroyRate(block);
			bestSlot = i;
		}
	}
	supplies->selectedHotbarSlot = oldSlot;
	if (block->blockLegacy->blockId != 0) {
		lastDestroyRate = destroyRate;
	} else {
		if (Mode.getSelectedValue() == 1 && Continues.getSelectedValue() == 1) {
			destroyRate = lastDestroyRate;
		}
	}
	return std::make_pair(bestSlot, destroyRate);
}

void PacketMine::Reset(GameMode* gm) {
	if (gm != nullptr) {
		*(float*)((__int64)gm + 0x24) = 0.f;
	}
	this->blockPos = Vec3i(0, 0, 0);
	this->face = 0;
}

void PacketMine::setBreakPos(GameMode* gm, Vec3i* blockPos, unsigned char face) {
	*(float*)((__int64)gm + 0x24) = 0.f;
	this->blockPos = blockPos;
	this->face = face;
	animDuration = 0.f;

}

void PacketMine::updateBreak(LocalPlayer* lp, GameMode* gm) {
	/*
	if (!DoWeHavePickaxe(lp)) {
		Notifications::addNotifBox("You need Pickaxe in hotbar to use PacketMine", 5.f);
		this->setEnabled(false);
	}
	*/
	if (lastHotbar != -1 && silentSwitch && !isHoldingPickaxe(lp) && silentBack) {
		MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(lastHotbar), lastHotbar, lastHotbar);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
		lastHotbar = -1;
	}
	if (blockPos != Vec3i(0, 0, 0)) {
		//*((char*)lp + 7500) = 1;
		float* breakProgress = (float*)((__int64)gm + 0x24);
		if (blockPos.toVec3t().dist(lp->eyePos0) > range) {
			this->Reset(gm);
			return;
		}
		Block* block = lp->region->getBlock(blockPos);
		if (block == nullptr) {
			this->Reset(gm);
			return;
		}
		std::pair<int, float> bestSlot = getBestPickaxeSlot(lp, gm, block);
		if (*breakProgress < 1.f) {
			*breakProgress += bestSlot.second * MineSpeed;
			if (*breakProgress > 1.f) *breakProgress = 1.f;
		} else {
			//if (strict && !lp->onGround && !lp->isInWater() && !lp->isInLava()) return;
			if (block->blockLegacy->blockId == 0) return;
			if (lp->getItemUseDuration() > 0 && !multiTask) return;
			if (isHoldingPickaxe(lp) || silentSwitch) {
				if (!isHoldingPickaxe(lp) && silentSwitch && silentBack) {
					lastHotbar = lp->getSupplies()->selectedHotbarSlot;
				}
				MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&pk);

				gm->destroyBlock(&blockPos, face);

				if (Mode.getSelectedValue() == 0) {
					this->Reset(gm);
				} else if (Mode.getSelectedValue() == 1) {
					*breakProgress = ResetValue;
				}
			}
		}
	}
}

void PacketMine::onDisable() {
	this->Reset(Game.getGameMode());
}
void PacketMine::onLevelRender() {
	if (blockPos == Vec3i(0, 0, 0)) return;
	GameMode* gm = Game.getGameMode();
	float breakProgress = *(float*)((__int64)gm + 0x24);
	Vec3 blockPosVec3 = blockPos.toVec3t();

	animDuration = smoothLerp(breakProgress, animDuration, ImGui::GetIO().DeltaTime * 10.f);
	if (!GrowRender) animDuration = 1.f;
	if (breakProgress > 0.f) {
		DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 1.f);
		DrawUtils::drawBox3d(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
		DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 0.2f);
		DrawUtils::drawBox3dFilled(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
	} else {
		animDuration = 0.f;
	}
}