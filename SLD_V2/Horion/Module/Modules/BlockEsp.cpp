#include "BlockEsp.h"

BlockEsp::BlockEsp() : IModule(0x0, Category::VISUAL, "Find valuable to stuff (to steal probably)") {
	this->registerIntSetting("radius", &this->range, this->range, 5, 75);
	registerBoolSetting("Outline shape", &this->outline, this->outline);
	this->registerFloatSetting("Outline Thickness", &this->thick, this->thick, 0.1f, 0.8f);

	registerBoolSetting("Regular Chests", &this->regChests, this->regChests);
	registerBoolSetting("Trapped Chests", &this->trapChests, this->trapChests);
	registerBoolSetting("Hoppers", &this->hoppers, this->hoppers);
	registerBoolSetting("Shulkers", &this->shulkers, this->shulkers);
	registerBoolSetting("EndChests", &this->endChests, this->endChests);

	registerBoolSetting("Emerald ore", &this->emerald, this->emerald);
	registerBoolSetting("Diamond ore", &this->diamond, this->diamond);
	registerBoolSetting("All Gold ores", &this->gold, this->gold);
	registerBoolSetting("Redstone ore", &this->redstone, this->redstone);
	registerBoolSetting("Lapis ore", &this->lapis, this->lapis);
	registerBoolSetting("Iron ore", &this->iron, this->iron);
	registerBoolSetting("Coal ore", &this->coal, this->coal);
	registerBoolSetting("Ancient Debris", &this->debris, this->debris);
	registerBoolSetting("Nether Quartz", &this->quartz, this->quartz);
	registerBoolSetting("Invisible Blocks", &this->invisBlocks, this->invisBlocks);
	registerBoolSetting("Portal Blocks", &this->portalBlocks, this->portalBlocks);
}

BlockEsp::~BlockEsp() {
}

const char* BlockEsp::getModuleName() {
	return ("BlockESP");
}

void BlockEsp::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		if (g_Data.isInGame()) {
			Odelay++;
			if (Odelay > delay * 20) {
				vec3_t* pos = g_Data.getLocalPlayer()->getPos();
				for (int x = (int)pos->x - range; x < pos->x + range; x++) {
					for (int z = (int)pos->z - range; z < pos->z + range; z++) {
						for (int y = (int)pos->y - range; y < pos->y + range; y++) {
							vec3_t blockPos = vec3_t(x, y, z);

							C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
							if (block != nullptr) {
								int blockId = block->toLegacy()->blockId;
								bool render0 = false;
								bool render1 = false;
								bool render2 = false;
								bool render3 = false;
								bool render4 = false;
								bool render5 = false;
								bool render6 = false;
								bool render7 = false;
								bool render8 = false;
								bool render9 = false;
								bool render10 = false;

								bool render14 = false;

								switch (blockId) {
								case 56:
									if (diamond) render0 = true;
									break;
								case 129:
									if (emerald) render1 = true;
									break;
								case 14:
									if (gold) render2 = true;
									break;
								case 536:
									if (gold) render2 = true;
									break;
								case 543:
									if (gold) render2 = true;
									break;
								case 73:
									if (redstone) render3 = true;
									break;
								case 74:
									if (redstone) render3 = true;
									break;
								case 21:
									if (lapis) render4 = true;
									break;
								case 15:
									if (iron) render5 = true;
									break;
								case 16:
									if (coal) render6 = true;
									break;
								case 526:
									if (debris) render7 = true;
									break;
								case 153:
									if (quartz) render8 = true;
									break;
								case 95:
									if (invisBlocks) render9 = true;
									break;
								case 416:
									if (invisBlocks) render9 = true;
									break;
								case 90:
									if (portalBlocks) render10 = true;
									break;
								case 119:
									if (portalBlocks) render10 = true;
									break;
								case 154:
									if (hoppers) render14 = true;
									break;
								}

								//diamond
								if (render0) {
									DrawUtils::setColor(0.f, 1.f, 1.f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//emerald
								if (render1) {
									DrawUtils::setColor(0.1f, 0.9f, 0.f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//gold
								if (render2) {
									DrawUtils::setColor(0.9f, 1.f, 0.1f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//redstone
								if (render3) {
									DrawUtils::setColor(1.f, 0.1f, 0, 1.f);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//lapis
								if (render4) {
									DrawUtils::setColor(0.1f, 0.2f, 0.7f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//iron
								if (render5) {
									DrawUtils::setColor(0.7f, 0.5f, 0.4f, 1.f);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//coal
								if (render6) {
									DrawUtils::setColor(0.f, 0.f, 0.f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//debris
								if (render7) {
									DrawUtils::setColor(0.5f, 0.2f, 0.f, 1.f);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//quartz
								if (render8) {
									DrawUtils::setColor(1.f, 1.f, 1.f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//invisBlocks
								if (render9) {
									DrawUtils::setColor(0.3f, 0.1f, 0.6f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
								//portals
								if (render10) {
									DrawUtils::setColor(0.9f, 0.4f, 1.f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}

								//Hoppers
								if (render14) {
									DrawUtils::setColor(0.f, 0.85f, 0.75f, 1);
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
								}
							}
						}
						Odelay = 0;
					}
				}
			}
		}
	}
}

void BlockEsp::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!g_Data.isInGame() || !GameData::canUseMoveKeys() || g_Data.getLocalPlayer() == nullptr)
		return;

	auto ourListLock = std::scoped_lock(this->listLock);

	for (const auto& chest : bufferedChestList) {
		auto storageID = g_Data.getLocalPlayer()->region->getBlock(chest.upper)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));
		math = math * 1.5f;
		DrawUtils::setColor(0, 0, 0, math);

		vec3_t blockPos = chest.lower;
		if (blockPos.x < 0)
			blockPos.x -= 1;
		if (blockPos.z < 0)
			blockPos.z -= 1;
		storageID = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;

		auto mathVect = vec3_t(chest.upper.floor().add(vec3_t(1.f, 1.f, 1.f)).sub(chest.upper));
		mathVect.y = floor(mathVect.y);

		bool doStorageRender = false;

		switch (storageID) {
		case 218:
			if (shulkers) {
				DrawUtils::setColor(0.f, 1.f, 0.f, math);  // purple shulks
				doStorageRender = true;
			}
			break;
		case 205:
			if (shulkers) {
				DrawUtils::setColor(0.f, 1.f, 0.f, math);  // assorted shulks
				doStorageRender = true;
			}
			break;
		case 54:
			if (regChests) {
				DrawUtils::setColor(1.f, 1.f, 1.f, math);  // Normal Chest
				doStorageRender = true;
			}
			break;
		case 146:
			if (trapChests) {
				DrawUtils::setColor(1.f, 1.f, 1.f, math);  // Trapped Chest
				doStorageRender = true;
			}
			break;
		case 130:
			if (endChests) {
				DrawUtils::setColor(0.435294f, 0.215686f, 0.631372f, math);  // Ender Chest
				doStorageRender = true;
			}
			break;
		}
		
		if (doStorageRender) DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, g_Data.getLocalPlayer()->eyePos0.dist(chest.lower))), true);  // Fancy math to give an illusion of good esp
	}
}

void BlockEsp::onTick(C_GameMode* gm) {
	// Swap list
	auto listLock = g_Data.lockChestList();
	auto& chestList = g_Data.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList.begin(), chestList.end());
	chestList.clear();
}
