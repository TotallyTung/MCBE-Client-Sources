#include "StrafeSpeed.h"

StrafeSpeed::StrafeSpeed() : IModule(0x0, Category::MOVEMENT, "You can use this to replace noslow, if needed") {
	registerFloatSetting("M/S", &this->MS, this->MS, 0.1f, 10.f);
}

StrafeSpeed::~StrafeSpeed() {
}

const char* StrafeSpeed::getModuleName() {
	return ("Strafe");
}

void StrafeSpeed::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if ((player->isInLava() || player->isInWater())) return;

	speed = (MS)*constantM;

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	float calcYaw = (player->yaw + 90) * (PI / 180);
	vec3_t moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = moveVec2d.y * speed;
	if (pressed) player->lerpMotion(moveVec);
	if (!pressed) {
		player->velocity.x = 0;
		player->velocity.z = 0;
	}
}
