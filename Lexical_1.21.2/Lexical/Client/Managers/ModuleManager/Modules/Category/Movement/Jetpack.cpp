#include "Jetpack.h"

Jetpack::Jetpack() : Module("Jetpack", "It JetPack :D", Category::MOVEMENT) {
	registerSetting(new SliderSetting<float>("Speed", "NULL", &speed, 1.f, 0.2f, 3.f));
}

void Jetpack::onNormalTick(LocalPlayer* localPlayer) {

	float calcYaw = (localPlayer->rotation->presentRot.y + 90.f) * (PI / 180.f);
	float calcPitch = (localPlayer->rotation->prevRot.x) * -(PI / 180.f);

	Vec3<float> moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
	moveVec.y = sin(calcPitch) * speed;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;

	localPlayer->lerpMotion(moveVec);
}