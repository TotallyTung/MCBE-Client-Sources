#include "Bhop.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 0.8f);
	registerFloatSetting("Up", &this->up, this->up, 0.0f, 0.5f);
	this->registerBoolSetting("LowHop", &this->lowhop, this->lowhop);
	this->registerBoolSetting("Step", &this->step, this->step);
	this->registerBoolSetting("BhopV2", &this->v2, this->v2);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Bhop");
}

void Bhop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (!v2) {
		if (player->isInLava() == 1 || player->isInWater() == 1)
			return;

		if (player->isSneaking())
			return;
	}
	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	if (pressed && player->onGround) {
		player->jumpFromGround();
	}
	if (lowhop && pressed && player->onGround && !step) {
		player->velocity.y = up;
		if (up != 0.0f) {
			player->velocity.y -= up / 3;
		} else {
			player->velocity.y -= up + 0.1f / 3;
		}
	} else if (lowhop && pressed && player->onGround) {
		player->velocity.y = up;
		player->velocity.y -= up / 3;
	}
	if (step) {
		player->stepHeight = 1.f;
	} else
		player->stepHeight = 0.5625f;
	if (!pressed) {
		player->velocity.x *= 0.f;
		player->velocity.z *= 0.f;
	}
	float calcYaw = (player->yaw + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = moveVec2d.y * speed;
	if (pressed) player->lerpMotion(moveVec);
}
void Bhop::onDisable() {
	auto player = Game.getLocalPlayer();
	*Game.getClientInstance()->minecraft->timer = 20.f;
	Game.getLocalPlayer()->stepHeight = 0.5625f;
	if (!player->onGround) {
		player->velocity.x = 0.f;
		player->velocity.z = 0.f;
	}
}
