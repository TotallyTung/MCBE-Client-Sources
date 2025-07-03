#include "HoleTP.h"
int health;
HoleTP::HoleTP() : IModule(0x0, Category::COMBAT, "Locate a good place to holecamp ;)") {
	registerBoolSetting("Obsidian Hole", &this->obsidian, this->obsidian);
	registerBoolSetting("Bedrock Hole", &this->bedrock, this->bedrock);
	registerIntSetting("TP Health", &health, 6, 0, 20);
	registerIntSetting("Range", &this->range, 1, 3.f, 15.f);
}
HoleTP::~HoleTP() {
}

const char* HoleTP::getModuleName() {
	return ("SafeTP");
}
bool tp;
void HoleTP::onTick(GameMode* gm) {
	if (!(Game.getLocalPlayer() == nullptr)) {
		auto player = Game.getLocalPlayer();
		if (Game.isInGame()) {
			Vec3* pos = gm->player->getPos();
			for (int x = (int)pos->x - range; x < pos->x + range; x++) {
				for (int z = (int)pos->z - range; z < pos->z + range; z++) {
					for (int y = (int)pos->y - range; y < pos->y + range; y++) {
						Vec4 underline;
						Vec3 blockPos = Vec3(x, y, z);
						Vec3 air = Vec3(x, y + 1, z);
						Vec3 air2 = Vec3(x, y + 2, z);
						Vec3 tpPos = Vec3(x + 1, y + 3, z + 1);
						tpPos.x -= 0.5f;
						tpPos.y -= 1;
						tpPos.z -= 0.5f;
						Vec3 blockPos2 = Vec3(x + 1, y, z);
						Vec3 blockPos3 = Vec3(x, y, z + 1);
						Vec3 blockPos4 = Vec3(x, y - 1, z);
						Vec3 blockPos5 = Vec3(x - 1, y, z);
						Vec3 blockPos6 = Vec3(x, y, z - 1);
						Vec3 blockPos22 = Vec3(x + 1, y + 1, z);
						Vec3 blockPos32 = Vec3(x, y + 1, z + 1);
						Vec3 blockPos52 = Vec3(x - 1, y + 1, z);
						Vec3 blockPos62 = Vec3(x, y + 1, z - 1);
						Block* block = Game.getLocalPlayer()->region->getBlock(blockPos);
						Block* airpos = Game.getLocalPlayer()->region->getBlock(air);
						Block* airpos2 = Game.getLocalPlayer()->region->getBlock(air2);
						Block* block2 = Game.getLocalPlayer()->region->getBlock(blockPos2);
						Block* block3 = Game.getLocalPlayer()->region->getBlock(blockPos3);
						Block* block4 = Game.getLocalPlayer()->region->getBlock(blockPos4);
						Block* block5 = Game.getLocalPlayer()->region->getBlock(blockPos5);
						Block* block6 = Game.getLocalPlayer()->region->getBlock(blockPos6);
						Block* block22 = Game.getLocalPlayer()->region->getBlock(blockPos22);
						Block* block32 = Game.getLocalPlayer()->region->getBlock(blockPos32);
						Block* block52 = Game.getLocalPlayer()->region->getBlock(blockPos52);
						Block* block62 = Game.getLocalPlayer()->region->getBlock(blockPos62);
						if (block != nullptr && block2 != nullptr && block3 != nullptr && block4 != nullptr) {
							int blockId = block->toLegacy()->blockId;
							int aircheck = airpos->toLegacy()->blockId;
							int aircheck2 = airpos2->toLegacy()->blockId;
							int blockId2 = block2->toLegacy()->blockId;
							int blockId3 = block3->toLegacy()->blockId;
							int blockId4 = block4->toLegacy()->blockId;
							int blockId5 = block5->toLegacy()->blockId;
							int blockId6 = block6->toLegacy()->blockId;
							int blockId22 = block22->toLegacy()->blockId;
							int blockId32 = block32->toLegacy()->blockId;
							int blockId52 = block52->toLegacy()->blockId;
							int blockId62 = block62->toLegacy()->blockId;
							bool obs = false;
							if (blockId == 0) obs = true;
							int healthy = Game.getLocalPlayer()->getHealth();
							if (obsidian) {
								if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0))) {
									if (healthy < health) {
										gm->player->setPos(tpPos);
										this->setEnabled(false);
									}
								}
							}
							if (bedrock) {
								if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)) {
									if (healthy < health) {
										gm->player->setPos(tpPos);
										this->setEnabled(false);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}