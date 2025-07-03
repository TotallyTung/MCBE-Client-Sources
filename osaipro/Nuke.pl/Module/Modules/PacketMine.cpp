#include "PacketMine.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"
bool percent = true;
PacketMine::PacketMine() : IModule(0, Category::PLAYER, "UrMomNigga") {
	registerBoolSetting("FastCancel", &FastCancel, FastCancel);
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	registerEnumSetting("ViewMode", &ViewMode, 2);
	ViewMode.addEntry("None", 0);
	ViewMode.addEntry("Outline", 1);
	ViewMode.addEntry("Filled", 2);
	ViewMode.addEntry("Both", 3);
	registerBoolSetting("Show Progress", &percent, true);
	registerFloatSetting("Mine Speed", &MineSpeed, MineSpeed, 0.5f, 2.f);
	registerBoolSetting("Grow Render", &GrowRender, GrowRender);
}

const char* PacketMine::getModuleName() {
	return "PacketMine";
}
bool PacketMine::DoWeHavePickaxe(LocalPlayer* lp) {
	PlayerInventoryProxy* supplies = lp->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
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
	for (int n = 0; n < 36; n++) {
		supplies->selectedHotbarSlot = n;
		if (gm->getDestroyRate(block) > destroyRate) {
			destroyRate = gm->getDestroyRate(block);
			bestSlot = n;
		}
	}
	supplies->selectedHotbarSlot = oldSlot;
	if (block->blockLegacy->blockId != 0) lastDestroyRate = destroyRate;
	return std::make_pair(bestSlot, destroyRate);
}

int getPickaxeSlot(LocalPlayer* lp, GameMode* gm, Block* block) {
	PlayerInventoryProxy* supplies = lp->getSupplies();
	Inventory* inv = supplies->inventory;

	int bestSlot = supplies->selectedHotbarSlot;
	int oldSlot = supplies->selectedHotbarSlot;
	float destroyRate = gm->getDestroyRate(block);
	for (int n = 0; n < 36; n++) {
		supplies->selectedHotbarSlot = n;
		if (gm->getDestroyRate(block) > destroyRate) {
			destroyRate = gm->getDestroyRate(block);
			bestSlot = n;
		}
	}
	supplies->selectedHotbarSlot = oldSlot;
	return bestSlot;
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
	if (Game.getLocalPlayer() != nullptr) {
		/*
		if (!DoWeHavePickaxe(lp)) {
			Notifications::addNotifBox("You need Pickaxe in hotbar to use PacketMine", 5.f);
			this->setEnabled(false);
		}
		*/
		if (lastHotbar != -1 && moduleMgr->getModule<Silent>()->isEnabled()) {
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
			if (moduleMgr->getModule<Desync>()->isEnabled() && (Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isUseable() && (Game.isRightClickDown() || moduleMgr->getModule<PacketGapple>()->isEnabled() && Game.getLocalPlayer()->getHealth() < moduleMgr->getModule<PacketGapple>()->health)))
				this->Reset(gm);
			if (*breakProgress < 1.f) {
				*breakProgress += bestSlot.second * MineSpeed;
				if (*breakProgress > 1.f) *breakProgress = 1.f;
			}
			else {
				//if (strict && !lp->onGround && !lp->isInWater() && !lp->isInLava()) return;
				if (block->blockLegacy->blockId == 0) this->Reset(gm);
				if (lp->getItemUseDuration() > 0 && !multiTask) return;
				lastHotbar = lp->getSupplies()->selectedHotbarSlot;
				lp->switchHotbar(getPickaxeSlot(lp, gm, block));
				MobEquipmentPacket pk(*lp->getUniqueId(), lp->getSupplies()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
				gm->destroyBlock(&blockPos, face);
				this->Reset(gm);
				if (moduleMgr->getModule<Silent>()->isEnabled()) {
					lp->switchHotbar(lastHotbar);
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
		if (ViewMode.getSelectedValue() == 1 || ViewMode.getSelectedValue() == 3) {
			if (ViewMode.getSelectedValue() == 1) DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 1.f);
			if (ViewMode.getSelectedValue() == 3) DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 0.12f);
			DrawUtils::drawBox3d(blockPosVec3, blockPosVec3.add(1.f), animDuration, true);
		}
		if (ViewMode.getSelectedValue() == 2 || ViewMode.getSelectedValue() == 3) {
			DrawUtils::setColor(1.f - 1.f * breakProgress, 1.f * breakProgress, 0.f, 0.12f);
			DrawUtils::drawBox3dFilled(blockPosVec3, blockPosVec3.add(1.f), animDuration, false, true);
		}
	}
	else {
		animDuration = 0.f;
	}
}
void PacketMine::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (blockPos == Vec3i(0, 0, 0)) return;
	GameMode* gm = Game.getGameMode();
	int breakProgress = *(float*)((__int64)gm + 0x24) * 100;
	if (breakProgress > 0) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << breakProgress;
		std::string currentProgress = oss.str() +"%";

		if (percent) DrawUtils::drawTextInWorld(&currentProgress, Vec3(blockPos.x, blockPos.y, blockPos.z), 0.7f, Vec3i(170, 170, 170), Vec3i(12, 12, 12), 0.f);
	}
}
