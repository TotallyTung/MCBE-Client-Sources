#include "FullAutoPiston.h"
using namespace std;

FullAutoPiston::FullAutoPiston() : IModule(0x0, Category::COMBAT, "FullAutoPiston Development by nanasi (Skidded By ReaQwQ)") {
};

const char* FullAutoPiston::getModuleName() {
	return "HoleKick+";
}
void getItem(int ItemID) {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 36; n++) {
		if (inv->getItemStack(n)->item != nullptr) {
			if (inv->getItemStack(n)->getItem()->itemId == ItemID) {
				supplies->selectedHotbarSlot = n;
				break;
			}
		}
	}
};
static vector<Entity*> tlap;
void findEntity5(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr) return;
	if (currentEntity == Game.getLocalPlayer()) return;
	if (!Target::isValidTarget(currentEntity)) return;
	tlap.push_back(currentEntity);;
}
bool Placehk(Vec3 tp) {
	tp = tp.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(tp));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(tp);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			Game.getGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}
void FullAutoPiston::onTick(GameMode* gm) {
	tlap.clear();
	LocalPlayer* player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.forEachEntity(findEntity5);
	const int deg = (int)(player->yaw + 180);
	int oDeg = (deg) % 360;
	if (oDeg < 0) oDeg += 360;
	/*
	315 ~ 45 Z- Anti”½‰ž // 315 ~ 45
	45 ~ 135 X+ Anti”½‰ž // 45 ~ 135
	135 ~ 225 Z+ Anti”½‰ž // 135 ~ 225
	225 ~ 315 X- Anti”½‰ž // 225 ~ 315
	*/
	if (!tlap.empty()) {
		for (Entity* Targets : tlap) {
			PlayerInventoryProxy* supplies = player->getSupplies();
			int origSlot = supplies->selectedHotbarSlot;
			//front Z-
			if (oDeg <= 45 || oDeg >= 315) {
				Vec3 blockSide1 = Targets->eyePos0;
				Vec3 blockSide2 = Targets->eyePos0;
				Vec3 blockSide3 = Targets->eyePos0;

				blockSide1.x -= Targets->height - 2;
				blockSide1.z -= Targets->height - 1;
				blockSide1.y -= Targets->height - 1;

				blockSide2.x -= Targets->height - 2;
				blockSide2.z += Targets->height - 3;
				blockSide2.y -= Targets->height - 2;

				blockSide3.x += Targets->height - 2;
				blockSide3.z -= Targets->height - 0;
				blockSide3.y -= Targets->height - 2;
				getItem(49);
				Placehk(blockSide1);
				getItem((29, 33));
				Placehk(blockSide2);
				getItem(152);
				Placehk(blockSide3);
				player->switchHotbar(origSlot);
			}
			//front X+
			if (oDeg <= 135 && oDeg >= 45) {
				Vec3 blockSide1 = Targets->eyePos0;
				Vec3 blockSide2 = Targets->eyePos0;
				Vec3 blockSide3 = Targets->eyePos0;

				blockSide1.x -= Targets->height - 3;
				blockSide1.z -= Targets->height - 2;
				blockSide1.y -= Targets->height - 1;

				blockSide2.x -= Targets->height - 3;
				blockSide2.z += Targets->height - 2;
				blockSide2.y -= Targets->height - 2;

				blockSide3.x += Targets->height - 0;
				blockSide3.z -= Targets->height - 2;
				blockSide3.y -= Targets->height - 2;
				getItem(49);
				Placehk(blockSide1);
				getItem((29, 33));
				Placehk(blockSide2);
				getItem(152);
				Placehk(blockSide3);
				player->switchHotbar(origSlot);
			}
			//front Z+
			if (oDeg <= 225 && oDeg >= 135) {
				Vec3 blockSide1 = Targets->eyePos0;
				Vec3 blockSide2 = Targets->eyePos0;
				Vec3 blockSide3 = Targets->eyePos0;

				blockSide1.x -= Targets->height - 2;
				blockSide1.z -= Targets->height - 3;
				blockSide1.y -= Targets->height - 1;

				blockSide2.x -= Targets->height - 2;
				blockSide2.z += Targets->height - 1;
				blockSide2.y -= Targets->height - 2;

				blockSide3.x += Targets->height - 2;
				blockSide3.z -= Targets->height - 4;
				blockSide3.y -= Targets->height - 2;
				getItem(49);
				Placehk(blockSide1);
				getItem((29, 33));
				Placehk(blockSide2);
				getItem(152);
				Placehk(blockSide3);
				player->switchHotbar(origSlot);
			}
			//front X-
			if (oDeg <= 315 && oDeg >= 225){
				Vec3 blockSide1 = Targets->eyePos0;
				Vec3 blockSide2 = Targets->eyePos0;
				Vec3 blockSide3 = Targets->eyePos0;

				blockSide1.x -= Targets->height - 1;
				blockSide1.z -= Targets->height - 2;
				blockSide1.y -= Targets->height - 1;

				blockSide2.x -= Targets->height - 1;
				blockSide2.z += Targets->height - 2;
				blockSide2.y -= Targets->height - 2;

				blockSide3.x += Targets->height - 4;
				blockSide3.z -= Targets->height - 2;
				blockSide3.y -= Targets->height - 2;
				getItem(49);
				Placehk(blockSide1);
				getItem((29, 33));
				Placehk(blockSide2);
				getItem(152);
				Placehk(blockSide3);
				player->switchHotbar(origSlot);
			}
		}
	}
}

void FullAutoPiston::onJoiningServer() {
	tlap.clear();
}