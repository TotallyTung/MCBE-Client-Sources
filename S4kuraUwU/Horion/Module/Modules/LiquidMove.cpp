#include "LiquidMove.h"

LiquidMove::LiquidMove() : IModule(0, Category::MOVEMENT, "Move better in liquids") {
	registerFloatSetting("M/S", &this->MS, this->MS, 0.1f, 10.f);
}

LiquidMove::~LiquidMove() {
}

const char* LiquidMove::getModuleName() {
	return ("LiquidMove");
}

/*
void LiquidMove::onTick(C_GameMode* gm) {
}
*/

void LiquidMove::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (!(player->isInLava() || player->isInWater())) return;

	speed = (MS) * constantM;

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
	if (g_Data.canUseMoveKeys()) {

		if (player->isInWater()) {
			if (input->isJumping) {
				player->velocity.y += speed / 11;
			}
			if (input->isSneakDown) {
				player->velocity.y -= speed / 11;
			}
		} 
		else {
			if (input->isJumping) {
				player->velocity.y += speed / 2;
			}
			if (input->isSneakDown) {
				player->velocity.y -= speed / 2;
			}
		}
	}
}

