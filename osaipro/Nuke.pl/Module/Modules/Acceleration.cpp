#include "Acceleration.h"
#include "../ModuleManager.h"
Acceleration::Acceleration() : IModule(0x0, Category::COMBAT, "Damage--->Acceleration") {
	registerIntSetting("MoveTick", &movetick, movetick, 0, 40);
	registerIntSetting("Pitch", &pitch, pitch, 0, 15);
	registerFloatSetting("Speed", &speed, speed, 0.0f, 6.8f);
};
Acceleration::~Acceleration() {
};
const char* Acceleration::getModuleName() {
	return "Acceleration";
};
void findEntityKasoku(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity->getEntityTypeId() != 71)
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < 6) {
		moduleMgr->getModule<Acceleration>()->targetListKasoku.push_back(currentEntity);
	}
}
void Acceleration::onTick(GameMode* gm) {
	targetListKasoku.clear();
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	Game.forEachEntity(findEntityKasoku);
	if (Game.getLocalPlayer()->damageTime > 1)
		damaged = true;
	if (damaged) {
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);
		delay++;

		if (delay <= movetick) {
			Vec3 pos = *Game.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			a = C_MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			gm->player->velocity.y = 0.465f;
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;

			gm->player->velocity.x = moveVec.x;
			gm->player->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

//			pos = *gm->player->getPos();
//			Game.getLocalPlayer()->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));
		}
		else if (delay > movetick) {
			damaged = false;
			delay = 0;
		};
	}
};