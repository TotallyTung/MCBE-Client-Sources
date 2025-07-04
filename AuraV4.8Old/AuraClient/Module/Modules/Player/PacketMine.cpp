#include "PacketMine.h"
#include "../../../../Utils/DrawUtils.h"
#include "../../ModuleManager.h"

PacketMine::PacketMine() : IModule(0, Category::PLAYER, "UrMomNigga") {
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	registerEnumSetting("RenderMode", &RenderMode, 0);
	RenderMode.addEntry("Out", 0);
	RenderMode.addEntry("In", 1);
	registerFloatSetting("Mine Speed", &MineSpeed, MineSpeed, 0.5f, 2.f);
	registerFloatSetting("Reset Value", &ResetValue, ResetValue, 0.f, 1.f);
	registerBoolSetting("Silent Switch", &silentSwitch, silentSwitch);
	registerBoolSetting("Silent Back", &silentBack, silentBack);
	registerBoolSetting("MultiTask", &multiTask, multiTask);
	//	registerBoolSetting("Strict", &strict, strict);
	registerBoolSetting("Rainbow", &rainbow, rainbow);
	registerBoolSetting("Grow Render", &GrowRender, GrowRender);
	registerFloatSetting("Opacity", &Opacity, Opacity, 0, 1);
}

const char* PacketMine::getModuleName() {
	return "PacketMine";
}

bool PacketMine::DoWeHavePickaxe(C_LocalPlayer* lp) {
	C_PlayerInventoryProxy* supplies = lp->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 9; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->isValid()) {
			if (stack->getItem()->isPickaxe()) return true;
		}
	}
	return false;
}

bool PacketMine::isHoldingPickaxe(C_LocalPlayer* lp) {
	C_ItemStack* stack = lp->getSelectedItem();
	if (!stack->isValid()) return false;
	if (stack->getItem()->isPickaxe()) return true;
	return false;
}

std::pair<int, float> PacketMine::getBestPickaxeSlot(C_LocalPlayer* lp, C_GameMode* gm, C_Block* block) {
	C_PlayerInventoryProxy* supplies = lp->getSupplies();
	C_Inventory* inv = supplies->inventory;

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
	}
	else {
		if (Mode.getSelectedValue() == 1 && Continues.getSelectedValue() == 1) {
			destroyRate = lastDestroyRate;
		}
	}
	return std::make_pair(bestSlot, destroyRate);
}

void PacketMine::Reset(C_GameMode* gm) {
	if (gm != nullptr) {
		*(float*)((__int64)gm + 0x24) = 0.f;
	}
	this->blockPos = vec3_ti(0, 0, 0);
	this->face = 0;
}

void PacketMine::setBreakPos(C_GameMode* gm, vec3_ti* blockPos, unsigned char face) {
	if (RenderMode.getSelectedValue() == 1) {
		*(float*)((__int64)gm + 0x24) = 0.9999f;
		animDuration = 1.f;
	}
	else if (RenderMode.getSelectedValue() == 0) {
		*(float*)((__int64)gm + 0x24) = 0;
		animDuration = 0.f;
	}
	this->blockPos = blockPos;
	this->face = face;
}

void PacketMine::updateBreak(C_LocalPlayer* lp, C_GameMode* gm) {
	/*
	if (!DoWeHavePickaxe(lp)) {
		Notifications::addNotifBox("You need Pickaxe in hotbar to use PacketMine", 5.f);
		setEnabled(false);
	}
	*/
	if (lastHotbar != -1 && silentSwitch && !isHoldingPickaxe(lp) && silentBack) {
		C_MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(lastHotbar), lastHotbar, lastHotbar);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
		lastHotbar = -1;
	}
	if (blockPos != vec3_ti(0, 0, 0) && g_Data.getLocalPlayer()->region->getBlock(blockPos)->blockLegacy->blockId != 0) {
		//*((char*)lp + 7500) = 1;
		float* breakProgress = (float*)((__int64)gm + 0x24);
		if (blockPos.toVector3().dist(lp->eyePos0) > range) {
			this->Reset(gm);
			return;
		}
		C_Block* block = lp->region->getBlock(blockPos);
		if (block == nullptr) {
			this->Reset(gm);
			return;
		}
		std::pair<int, float> bestSlot = getBestPickaxeSlot(lp, gm, block);
		if (RenderMode.getSelectedValue() == 1) {
			if (*breakProgress > 0.f && *breakProgress != 1.f) {
				*breakProgress -= bestSlot.second * MineSpeed;
				if (*breakProgress < 0.f) {
					*breakProgress = 1.f;
				}
			}
			else {
				// if (strict && !lp->onGround && !lp->isInWater() && !lp->isInLava()) return;
				if (block->blockLegacy->blockId == 0) return;
				if (lp->getC_ItemUseDuration() > 0 && !multiTask) return;
				if (isHoldingPickaxe(lp) || silentSwitch) {
					if (!isHoldingPickaxe(lp) && silentSwitch && silentBack) {
						lastHotbar = lp->getSupplies()->selectedHotbarSlot;
					}
					C_MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pk);

					gm->destroyBlock(&blockPos, face);
					this->Reset(gm);
				}
			}
		}
		else if (RenderMode.getSelectedValue() == 0) {
			if (*breakProgress < 1.f) {
				*breakProgress += bestSlot.second * MineSpeed;
				if (*breakProgress > 1.f) {
					*breakProgress = 1.f;
				}
			}
			else {
				// if (strict && !lp->onGround && !lp->isInWater() && !lp->isInLava()) return;
				if (block->blockLegacy->blockId == 0) return;
				if (lp->getC_ItemUseDuration() > 0 && !multiTask) return;
				if (isHoldingPickaxe(lp) || silentSwitch) {
					if (!isHoldingPickaxe(lp) && silentSwitch && silentBack) {
						lastHotbar = lp->getSupplies()->selectedHotbarSlot;
					}
					C_MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pk);

					gm->destroyBlock(&blockPos, face);
					this->Reset(gm);
				}
			}
		}
	}
}

void PacketMine::onDisable() {
	this->Reset(g_Data.getCGameMode());
}
void PacketMine::onLevelRender() {
	if (blockPos == vec3_ti(0, 0, 0)) return;
	C_GameMode* gm = g_Data.getCGameMode();
	float breakProgress = *(float*)((__int64)gm + 0x24);
	vec3_t blockPosVec3 = blockPos.toVector3();
	C_LocalPlayer* lp = g_Data.getLocalPlayer();
	if (lp == nullptr) return;
	if (lp->region->getBlock(blockPosVec3)->toLegacy()->blockId == 0) return;
	if (lp->region->getBlock(blockPosVec3)->toLegacy()->blockId == 7) return;
	animDuration = smoothLerp(breakProgress, animDuration, DrawUtils::deltaTime * 10.f);
	if (!GrowRender)
		animDuration = 1.f;
	if (RenderMode.getSelectedValue() == 1) {
		if (breakProgress > 0.f) {
			if (breakProgress == 1) return;
			if (rainbow) {
				DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, Opacity);
				DrawUtils::drawBox3d2(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
				DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, Opacity);
				DrawUtils::drawBox3dFilled2(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
			}
			else {
				DrawUtils::setColor(ColorR, ColorG, ColorB, Opacity);
				DrawUtils::drawBox3d2(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
				DrawUtils::setColor(ColorR, ColorG, ColorB, Opacity - 0.2f);
				DrawUtils::drawBox3dFilled2(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
			}

		}
		else {
			animDuration = 0.f;
		}
	}
	else if (RenderMode.getSelectedValue() == 0) {
		if (breakProgress > 0.f) {
			if (rainbow) {
				DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 1.f);
				DrawUtils::drawBox3d2(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
				DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 0.2f);
				DrawUtils::drawBox3dFilled2(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
			}
			else {
				DrawUtils::setColor(ColorR, ColorG, ColorB, Opacity);
				DrawUtils::drawBox3d2(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
				DrawUtils::setColor(ColorR, ColorG, ColorB, Opacity - 0.2f);
				DrawUtils::drawBox3dFilled2(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
			}

		}
		else {
			animDuration = 0.f;
		}
	}
}