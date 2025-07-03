#include "2b2espeed.h"
NukeSpeed::NukeSpeed() : IModule(0, Category::MOVEMENT, "Nah idk ;-;") {
	registerFloatSetting("Speed", &speed, speed, 0.1f, 1.f);
}

NukeSpeed::~NukeSpeed() {
}

const char* NukeSpeed::getModuleName() {
	return ("2B2ESpeed");
}

void NukeSpeed::onMove(MoveInputHandler* input) {
	auto slot = Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
	bool shouldeat = slot != nullptr && slot->item != nullptr && slot->getItem()->isFood() && Game.isRightClickDown();
	LocalPlayer* plr = Game.getLocalPlayer();

	float yaw = plr->yaw;
	{
		GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
		if (input == nullptr)
			return;

		if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
			return;
		else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 45.f;
		}
		else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 45.f;
		}
		else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 135.f;
		}
		else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 135.f;
		}
		else if (GameData::isKeyDown(*input->forwardKey)) {
		}
		else if (GameData::isKeyDown(*input->backKey)) {
			yaw += 180.f;
		}
		else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 90.f;
		}
		else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 90.f;
		}
		if (yaw >= 180)
			yaw -= 360.f;
	}

	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	bool pressed = input->forward || input->backward || input->right || input->left;


	if (pressed && player->onGround) {
		player->jumpFromGround();
	}
	if (pressed && player->onGround) {
		player->velocity.y = 0.0000001;
		player->velocity.y -= 0.0000001 / 3;

	}

	player->setSprinting(true);

	float calcYaw2 = (yaw + 90) * (PI / 180);
	Vec3 moveVec2;
	float currSpeed = player->onGround ? 0.33 : 0.36;
	bool bound = false;
	if (!player->isInLava() && !player->isInWater() && !player->isSneaking() && !shouldeat && player->onGround) currSpeed = 0.42;
	else if (!player->isInLava() && !player->isInWater() && shouldeat && !player->isSneaking()) currSpeed = 0.36;
	else if (player->isSneaking() && !shouldeat) currSpeed = 0.39f;
	else if (player->isSneaking() && shouldeat) currSpeed = 0.35f;
	else if (!player->isInLava() && !player->isInWater() && !player->isSneaking() && !shouldeat && !player->onGround && !bound) {
		currSpeed = 0.42;
		bound = true;
	}
	else currSpeed = 0.33f;
	if (bound) {
		currSpeed *= 0.94;
		if (player->onGround) bound = false;
	}
	moveVec2.x = cos(calcYaw2) * currSpeed;
	moveVec2.y = player->velocity.y;
	moveVec2.z = sin(calcYaw2) * currSpeed;
	if (pressed) player->lerpMotion(moveVec2);

}

void NukeSpeed::onTick(GameMode* gm) {
	auto atsnd = moduleMgr->getModule<AutoSneak>();
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (GameData::isKeyDown(VK_SHIFT) || atsnd->isEnabled()) {
		speedreal = sneakspeed;
	}
	else {
		speedreal = speed;
	}
}
void NukeSpeed::onDisable() {
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}
