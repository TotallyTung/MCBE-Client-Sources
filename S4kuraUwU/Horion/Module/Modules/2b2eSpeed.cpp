#include "2b2eSpeed.h"

The2b2eSpeed::The2b2eSpeed() : IModule(0, Category::MOVEMENT, "Bypassed speed on 2b2e") {
}

The2b2eSpeed::~The2b2eSpeed() {
}

const char* The2b2eSpeed::getModuleName() {
	return ("2b2eSpeed");
}

void The2b2eSpeed::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
		if (player->isInLava() == 1 || player->isInWater() == 1)
			return;

		if (player->isSneaking())
			return;

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	if (pressed && player->onGround) {
		player->jumpFromGround();
	}
	if (lowhop && pressed && player->onGround && !step) {
		player->velocity.y = 0.00001;
			player->velocity.y -= 0.00001 / 3;
		

	} else if (lowhop && pressed && player->onGround) {
		player->velocity.y = 0.00001;
		player->velocity.y -= 0.00001 / 3;
	}

	if (!pressed) {
		player->velocity.x *= 0.f;
		player->velocity.z *= 0.f;
	}
	float calcYaw = (player->yaw + 90) * (PI / 180);
	vec3_t moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	player->setSprinting(true);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * 0.34f;
	moveVec.y = player->velocity.y;
	moveVec.z = moveVec2d.y * 0.34f;
	*g_Data.getClientInstance()->minecraft->timer = 25.f;

	if (pressed) player->lerpMotion(moveVec);

	float yaw = player->yaw;

	//bool pressed = input->forward || input->backward || input->right || input->left;

	if (input->forward && input->backward)
		return;

	if (input->right) {
		yaw += 90.f;
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
	}
	if (input->left) {
		yaw -= 90.f;
		if (input->forward)
			yaw += 45.f;
		else if (input->backward)
			yaw -= 45.f;
	}

	if (input->backward && !input->left && !input->right)
		yaw += 180.f;

	float calcYaw2 = (yaw + 90) * (PI / 180);
	vec3_t moveVec2;
	moveVec2.x = cos(calcYaw2) * 0.34f;
	moveVec2.y = player->velocity.y;
	moveVec2.z = sin(calcYaw2) * 0.34f;
	if (pressed) player->lerpMotion(moveVec);
}
void The2b2eSpeed::onDisable() {
	auto player = g_Data.getLocalPlayer();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;

	if (!player->onGround) {
		player->velocity.x = 0.f;
		player->velocity.z = 0.f;
	}
}
