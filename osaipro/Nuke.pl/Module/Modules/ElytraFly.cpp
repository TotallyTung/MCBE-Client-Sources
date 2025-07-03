#include "ElytraFly.h"
float speed = 2.11f;
int delay = 0;
int delay2 = 0;
ElytraFly::ElytraFly() : IModule(0, Category::MOVEMENT, "Elytra Flying") {
	registerFloatSetting("Speed", &speed, speed, 0.1f, 2.11f);
	registerFloatSetting("UP&Down Speed", &upanddown, upanddown, 0.1f, 1.f);
}
ElytraFly::~ElytraFly() {
}
const char* ElytraFly::getModuleName() {
	return ("HummingBird");
}
void ElytraFly::onKey(int key, bool isDown, bool& shouldCancel) {
	if (Game.getLocalPlayer() == nullptr) return;
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (flying && key == *input->spaceBarKey) {
		shouldCancel = true;
		return;
	}
}
void ElytraFly::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (Game.getLocalPlayer()->isGliding()) flying = true;
	else flying = false;
	if (flying) {
		gm->player->velocity = Vec3(0, 0, 0);
		glideModEffective = glideMod;
		if (Game.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				glideModEffective += upanddown;
			if (GameData::isKeyDown(*input->sneakKey))
				glideModEffective -= upanddown;
		}
		gm->player->velocity.y = glideModEffective;
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);
		delay++;
		delay2++;

		if (delay >= 2) {
			gm->player->velocity.y = glideModEffective + 0.2f;
			delay = 0;
		}
		if (delay2 >= 4) {
			gm->player->velocity.y = glideModEffective - 0.14f;
			delay2 = 0;
		};
	}
}
void ElytraFly::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (flying) {
		auto lv = Game.getLocalPlayer()->getlevel();
		float yaw = player->yaw;
		bool pressed = input->forward || input->backward || input->right || input->left;

		if (input->forward && input->backward)
			return;
		if (input->right && input->left)
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
		float calcYaw = (yaw + 90) * (PI / 180);
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = sin(calcYaw) * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}
