#include "NoClip.h"

NoClip::NoClip() : IModule(0x0, Category::MOVEMENT, "NoClip through walls on all axis") {
	registerFloatSetting("Horizontal Speed", &this->speed, this->speed, 0.1f, 2.5f);
	registerFloatSetting("Vertical Speed", &this->upanddown, this->upanddown, 0.1f, 2.5f);
	this->registerBoolSetting("Save Old POS", &this->NoPacket, this->NoPacket);
}

NoClip::~NoClip() {
}

const char* NoClip::getModuleName() {
	return ("NoClip");
}

void NoClip::onTick(GameMode* gm) {
	gm->player->aabb.upper.y = gm->player->aabb.lower.y - (float)1.8f;
	gm->player->velocity = Vec3(0, 0, 0);
	glideModEffective = glideMod;
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

	if (Game.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += upanddown;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= upanddown;
	}
	gm->player->velocity.y = glideModEffective;
}

void NoClip::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;


	float yaw = player->yaw;

	bool pressed = input->forward || input->backward || input->right || input->left;

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

	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3 moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if(pressed) player->lerpMotion(moveVec);
}

void NoClip::onEnable() {
	auto jp = moduleMgr->getModule<Jetpack>();
	if (jp->isEnabled()) jp->setEnabled(false);
	if (NoPacket)
	if (Game.getLocalPlayer() != nullptr) {
		oldPos = *Game.getLocalPlayer()->getPos();
	}
}

void NoClip::onDisable() {
	if (Game.getLocalPlayer() != nullptr)
		Game.getLocalPlayer()->aabb.upper.y = Game.getLocalPlayer()->aabb.lower.y + (float)1.8f;
	if (NoPacket)
		Game.getLocalPlayer()->setPos(oldPos);
}

