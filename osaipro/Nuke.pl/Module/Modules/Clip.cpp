 #include "Clip.h"
#include "Notifications.h"
#include "../../DrawUtils.h"
int key = false;
float offset = 0.19f;
bool ThroughWall = true;
Clip::Clip() : IModule(0x0, Category::MOVEMENT, "2b2e moment") {
	registerFloatSetting("Reduce", &offset, 0.09f, 0.f, 0.2f);
	registerKeybindSetting("ThroughKey", &key, key);
	registerBoolSetting("ThroughWall", &ThroughWall, true);
};

const char* Clip::getModuleName() {
	return "Clip";
}
const char* Clip::getModName() {
	std::string k = "ThroughKey : N/A";
	if (key != 0) k = ("ThroughKey : " + std::string(Utils::getKeybindName(key)));
	return k.c_str();
};
std::vector<Vec3> sidepos = {
Vec3(0,0,1),
Vec3(0,0,-1),
Vec3(1,0,0),
Vec3(-1,0,0)
};
void Clip::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	Vec3 playerPos = *Game.getLocalPlayer()->getPos();
	float offsets = offset + 0.1f;
	gm->player->aabb.upper.x = playerPos.x + offsets;
	gm->player->aabb.upper.z = playerPos.z + offsets;
	gm->player->aabb.lower.x = playerPos.x - offsets;
	gm->player->aabb.lower.z = playerPos.z - offsets;
}
void Clip::onDisable() {
	if (Game.getLocalPlayer() == nullptr) return;
	GameMode* gm = Game.getLocalPlayer()->getGameMode();
	Vec3 playerPos = *Game.getLocalPlayer()->getPos();
	float offsets = offset + 0.1f;
	gm->player->aabb.upper.x = playerPos.x;
	gm->player->aabb.upper.z = playerPos.z;
	gm->player->aabb.lower.x = playerPos.x;
	gm->player->aabb.lower.z = playerPos.z;
}
void Clip::onKeyUpdate(int keyb, bool isDown) {
	if (!Game.canUseMoveKeys()) return;
	if (!isDown) return;
	if (keyb == this->getKeybind()) {
		this->toggle();
	}
	if (!this->isEnabled()) return;
	if (Game.getLocalPlayer() == nullptr) return;
	Vec3 playerPos = Game.getLocalPlayer()->getHumanPos();
	float offset = Game.getLocalPlayer()->width / 2;
	AABB realaabb(Vec3(playerPos.x - offset, playerPos.y, playerPos.z - offset), Vec3(playerPos.x + offset, playerPos.y + 1.8f, playerPos.z + offset));
	int index = 0;
	Vec3 a[2];
	Vec3 result;
	if (!ThroughWall) return;
	if (keyb == key) {
		for (auto side : sidepos) {
			Vec3 getPos = side.add(playerPos.floor());
			AABB blockAABB(getPos, getPos.add(1));
			if (Game.getLocalPlayer()->region->getBlock(getPos)->blockLegacy->material->isSolid) {
				if (realaabb.intersects(blockAABB)) {
					a[index] = side;
					++index;
				}
			}
			else if (Game.getLocalPlayer()->region->getBlock(getPos.add(0, 1, 0))->blockLegacy->material->isSolid) {
				AABB aabb(getPos.add(0, 1, 0), getPos.add(0, 1, 0).add(1));
				if (realaabb.intersects(aabb)) {
					a[index] = side;
					++index;
				}
			}
			if (index >= 2) break;
		}
		if (index < 2) return;
		result = a[0].add(a[1]);
		result = result.add(0.5 - (a[0].x + a[1].x) * 0.25, 0, 0.5 - (a[0].z + a[1].z) * 0.25);
		result.x += Game.getLocalPlayer()->getPos()->floor().x;
		result.z += Game.getLocalPlayer()->getPos()->floor().z;
		result.y = Game.getLocalPlayer()->getPos()->y;
		if (result.iszero()) return;
		if (Game.getLocalPlayer()->region->getBlock(a[0].add(a[1]).add(Game.getLocalPlayer()->getHumanPos().floor()))->blockLegacy->blockId != 0) return;
		if (Game.getLocalPlayer()->region->getBlock(a[0].add(a[1]).add(Game.getLocalPlayer()->getPos()->floor()))->blockLegacy->blockId != 0) return;
		Game.getLocalPlayer()->setPos(result);
		Notifications::addNotifBox("You Went Through Wall!", 5.f);
	};
}