#include "Jetpack.h"

#include "../../../Utils/Logger.h"

Jetpack::Jetpack() : IModule('F', Category::MOVEMENT, "Fly around like you had a Jetpack!") {
	registerFloatSetting("speed", &speedMod, 1, 0.2f, 3.f);
	registerBoolSetting("Bypass", &isBypass, isBypass);
	//registerBoolSetting("Hold", &hold, hold);
}

Jetpack::~Jetpack() {
}

void Jetpack::onEnable() {
	auto nc = moduleMgr->getModule<NoClip>();
	if (nc->isEnabled()) nc->setEnabled(false);
}
/*
bool Jetpack::isHoldMode() {
	if (hold)
		return true;
	else {
		return false;
	}
}
*/

const char* Jetpack::getModuleName() {
	return ("Jetpack");
}

void Jetpack::onTick(GameMode* gm) {
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);

	if (!isBypass) {
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
		moveVec.y = sin(calcPitch) * speedMod;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

		gm->player->lerpMotion(moveVec);
	} else {
		delay++;

		if (delay >= 5) {
			Vec3 pos = *Game.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = C_MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			gm->player->velocity.y = 0.465f;
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

			gm->player->velocity.x = moveVec.x;
			gm->player->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			Game.getLocalPlayer()->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			gm->player->velocity.y -= 0.15f;
			delay = 0;
		}
	}
}