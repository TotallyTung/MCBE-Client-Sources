#include "PacketMine.h"

PacketMine::PacketMine() : IModule(0, Category::WORLD, "Geyser Packet Mine - idk if it works on other servers") {
	registerIntSetting("Mine Range", &this->mineRange, this->mineRange, 3, 10);
	registerIntSetting("Addt. Wait", &this->addtWait, this->addtWait, 0, 60);
	registerIntSetting("Default Time", &this->defMineTime, this->defMineTime, 1, 180);
	registerIntSetting("Pick Time", &this->pickTime, this->pickTime, 1, 40);
}

PacketMine::~PacketMine() {
}

const char* PacketMine::getModuleName() {
	return "PacketMine";
}

void PacketMine::onEnable() {
	displayBlock = 0;
	hasBlock = false;
	untilMine = 0;
}

bool isPointBlockGood() {
	Level* pstruct = g_Data.getLocalPlayer()->getlevel();
	int id = g_Data.getLocalPlayer()->region->getBlock(pstruct->block)->toLegacy()->blockId;

	int pmr = moduleMgr->getModule<PacketMine>()->mineRange;
	return (id != 7 && pstruct->block.toVec3t().dist(g_Data.getLocalPlayer()->getHumanPos()) < pmr);  //(id == 145 || id == 49 || id == 130);
}

void getPickaxePM() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318 || stack->getItem()->itemId == 606) {  // select pick
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

bool doIHaveAPick() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318) {  // there's a diamond pick
				return true;
			} else if (stack->getItem()->itemId == 606) {  // there's a netherite pick
				return true;
			}
		}
	}

	return false;
}

int PacketMine::ticksToMine(vec3_ti toMine) {

	float speedMultiplier;
	int pickSlot;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID


	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318) {  // there's a diamond pick
				pickSlot = n;
				speedMultiplier = 8;
			} else if (stack->getItem()->itemId == 606) {  // there's a netherite pick
				pickSlot = n;
				speedMultiplier = 9;
			}
		}
	}

	if (speedMultiplier != 8 && speedMultiplier != 9)
		return -1;

	// look at the pick's efficiency level
	speedMultiplier += powf((inv->getItemStack(pickSlot)->getEnchantValue(15)), 2) + 1;

	int id = g_Data.getLocalPlayer()->region->getBlock(toMine)->toLegacy()->blockId;
	float blockHardness;
	bool correctBlk = false;

	switch (id) {
	case 145:
		blockHardness = 5.f;
		correctBlk = true;
		break;
	case 49:
		blockHardness = 170.f;
		correctBlk = true;
		break;
	case 130:
		blockHardness = 73.f;
		correctBlk = true;
		break;
	}

	if (!correctBlk)
		return defMineTime;

	float damage = speedMultiplier / blockHardness;
	damage /= 30;
	//clientMessageF("%i %i %i", (int)(round(1 / damage)), addtWait, (int)(round(1 / damage) + addtWait));
	return (int)(round(1 / damage) + addtWait);
}


int origSlut;
bool pickNow = false;
bool hasClicked = false;
bool hasMeasuredOldBlock = false;
void PacketMine::onWorldTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (!doIHaveAPick()) {
		return;
		//clientMessageF("Pickaxe not detected! Disabling...");
		//this->setEnabled(false);
	}

	if (g_Data.getLocalPlayer()->getlevel() != nullptr && g_Data.getLocalPlayer()->getlevel()->rayHitType == 0) {	
		if (isPointBlockGood()) {

			if (g_Data.isLeftClickDown() && !hasClicked) {
				Level* pstruct = g_Data.getLocalPlayer()->getlevel();

				currentBlock = pstruct->block;
				//clientMessageF("%i %i %i", currentBlock.x, currentBlock.y, currentBlock.z);
				pickNow = false;
				hasBlock = true;
				hasClicked = true;
			}
			if (hasClicked && !g_Data.isLeftClickDown()) {
				hasClicked = false;
			}
			
		}
	}

	if (currentBlock.toVec3t().dist(g_Data.getLocalPlayer()->getHumanPos()) > mineRange && (hasBlock || displayBlock != 0)) {
		//clientMessageF("Block is too far! Clearing...");
		currentBlock = vec3_ti(999410, -420622, -23494);
		pickNow = false;
		hasBlock = false;
		displayBlock = 0;
		mDel = 0;
		hasMeasuredOldBlock = false;
	}

	if (!hasBlock && displayBlock == 0)
		return;

	if (!hasMeasuredOldBlock) {
		mineTime = ticksToMine(currentBlock);
		hasMeasuredOldBlock = true;
	}
	// clientMessageF("%i",  mineTime);
	if (mineTime == -1) {
		currentBlock = vec3_ti(999410, -420622, -23494);
		pickNow = false;
		hasBlock = false;
		displayBlock = 0;
		mDel = 0;
		hasMeasuredOldBlock = false;
		return;
	}

	if (hasBlock) {
		displayBlock = 1;
		hasBlock = false;
	}

	if (displayBlock == 1) {
		if (mDel > mineTime - 5) {
			displayBlock = 2;
		}
	}

	if (displayBlock == 2) {
		if (mDel > mineTime) {
			displayBlock = 0;
			mDel = 0;
			currentBlock = vec3_ti(999410, -420622, -23494);
			origSlut = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
			pickNow = true;
			hasMeasuredOldBlock = false;
			//getPickaxePM();
			//g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlut;
			return;
		}
	}

	mineNow = true;
	
		/*
	if (hasBlock || displayBlock != 0) {
		if (!mineNow)
			mineNow = true;
		else
			mineNow = false;
	}*/

	mDel++;
	return;
}

int hasPick = 0;
void PacketMine::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr || (!mineNow && !pickNow))
		return;

	if (mineNow) {
		if (currentBlock != vec3_ti(999410, -420622, -23494))
			gm->destroyBlock(&currentBlock, 0);
	}

	if (pickNow && hasPick < pickTime) {
		getPickaxePM();
		hasPick++;
		return;
	}
	if (hasPick >= pickTime) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlut;
		hasPick = 0;
		pickNow = false;
	}
	return;
}


void PacketMine::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || displayBlock == 0) return;

	if (displayBlock == 1) {
		DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
		DrawUtils::drawBox(currentBlock.toVec3t(), currentBlock.toVec3t().add(1), .4f, false);
	}
	if (displayBlock == 2) {
		DrawUtils::setColor(1.f, 0.f, 0.f/* .5251902f, .0823529f, .6196078f*/, 1.f);  // 134,21,158
		DrawUtils::drawBox(currentBlock.toVec3t(), currentBlock.toVec3t().add(1), .4f, true);
	}
}
