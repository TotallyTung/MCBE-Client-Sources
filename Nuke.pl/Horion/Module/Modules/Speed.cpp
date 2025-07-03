#include "Speed.h"

Speed::Speed() : IModule(0x0, Category::MOVEMENT, "Optimize your movement") {
	registerBoolSetting("SetSprint", &sprint, false);
	registerFloatSetting("Ground Speed", &groundSpeed, 0.1f, 0.01f, 0.5f);
	registerFloatSetting("Lava Speed", &lavaSpeed, 0.1f, 0.01f, 0.5f);
	registerFloatSetting("Lava Sneak Speed", &lavaSneakSpeed, 0.01f, 0.1f, 0.5f);
	registerFloatSetting("Air Speed", &airSpeed, 0.1f, 0.01f, 0.5f);
	registerFloatSetting("Air Sneak Speed", &sneakAirSpeed, 0.01f, 0.1f, 2.0f);
	registerFloatSetting("Sneak Speed", &sneakSpeed, 0.1f, 0.01f, 2.0f);
	registerFloatSetting("Water Speed", &waterspeed, 0.1f, 0.01f, 0.5f);
	registerFloatSetting("Water Sneak Speed", &waterSneakSpeed, 0.1f, 0.01f, 0.5f);
};

Speed::~Speed() {

};

const char* Speed::getModuleName() {
	return "Speed";
};

void Speed::onTick(GameMode* gm) {

};

void Speed::onMove(MoveInputHandler* input) {
	Vec3 moveVec;
	Vec2 moveVec2d = { input->forwardMovement, -input->sideMovement };
	LocalPlayer* player = Game.getLocalPlayer();
	auto moveinput = Game.getClientInstance()->getMoveTurnInput();
	if (player != nullptr && player->isAlive()) {

		float calcYaw = (player->yaw + 90) * (PI / 180);
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };

		player->setSprinting(sprint);
		if (player->isInLava() && !input->isSneakDown) {
			moveVec.x = moveVec2d.x * lavaSpeed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * lavaSpeed;
		}
		if (player->isInLava() && input->isSneakDown) {
			moveVec.x = moveVec2d.x * lavaSneakSpeed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * lavaSneakSpeed;
		}
		if (input->isSneakDown && player->onGround) {
			moveVec.x = moveVec2d.x * sneakSpeed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * sneakSpeed;
		}
		if (!player->onGround && !input->isSneakDown && !player->isInLava() && !player->isInWater()) {
			moveVec.x = moveVec2d.x * airSpeed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * airSpeed;
		}
		if (input->isSneakDown && !player->onGround) {
			moveVec.x = moveVec2d.x * sneakAirSpeed;
			moveVec.z = moveVec2d.y * sneakAirSpeed;
			moveVec.y = player->velocity.y;
		}
		if (player->onGround && !input->isSneakDown) {
			moveVec.x = moveVec2d.x * groundSpeed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * groundSpeed;
		}
		if (input->isPressed()) player->lerpMotion(moveVec);
	}
};

void Speed::onDisable() {

};

void Speed::onEnable() {

};