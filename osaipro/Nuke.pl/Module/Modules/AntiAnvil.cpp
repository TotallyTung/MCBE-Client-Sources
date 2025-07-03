#include "AntiAnvil.h"
AntiAnvil::AntiAnvil() : IModule(0x0, Category::PLAYER, "No FallBlock!") {
}
std::string blockname = "None";
const char* AntiAnvil::getModName() {
	std::string name = std::string(blockname);
	return name.c_str();
}
const char* AntiAnvil::getModuleName() {
	return ("AntiAnvil");
} 
bool DetectPiston()  {
	Vec3 plrPos = Game.getLocalPlayer()->getPos()->floor();
	if (((int)Game.getLocalPlayer()->region->getBlock(Vec3(plrPos.x, plrPos.y - 1, plrPos.z))->toLegacy()->blockId) == (29,33,34)) return true;
	if (((int)Game.getLocalPlayer()->region->getBlock(Vec3(plrPos.x, plrPos.y, plrPos.z))->toLegacy()->blockId) == (29,33,34)) return true;
	return false;
};
bool Detected = false;
void AntiAnvil::onMove(MoveInputHandler* input) {
	if (Detected)
		return;
}
void AntiAnvil::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (DetectPiston()) {
		gm->player->aabb.upper.y = gm->player->aabb.lower.y - (float)1.8f;
		gm->player->velocity = Vec3(0, 0, 0);
		gm->player->velocity.y = gm->player->velocity.y;
		Detected = true;
	}
	else blockname = "None";

	if (!DetectPiston() && Detected) 
		Game.getLocalPlayer()->aabb.upper.y = Game.getLocalPlayer()->aabb.lower.y + (float)1.8f;
};
void AntiAnvil::onLevelRender() {}