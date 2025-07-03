 #include "2b2espeed.h"
bool StopOnEat = true;
NukeSpeed::NukeSpeed() : IModule(0, Category::MOVEMENT, "Nah idk ;-;") {
	registerFloatSetting("Speed", &speed, speed, 0.1f, 1.f);
	registerBoolSetting("Stop on Eat", &StopOnEat, StopOnEat);
	registerBoolSetting("DamageBoost", &DamageBoosts, DamageBoosts);
	registerFloatSetting("Boost-Speed", &boostspeed, 1.3f, 0.1f, 2.f);
	registerFloatSetting("Boost-Timer", &boosttimer, 24, 20, 50);
	registerIntSetting("Boost-MaxTick", &boostmaxtick, 22, 0, 25);
	registerIntSetting("Boost-MinTick", &boostmintick, 22, 0, 25);
}

NukeSpeed::~NukeSpeed() {
}

const char* NukeSpeed::getModuleName() {
	return ("2B2ESpeed");
}

void NukeSpeed::onMove(MoveInputHandler* input) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	if (StopOnEat && moduleMgr->AutoDesync()) return;
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
int delaFADSF7V6AS5DF67ASV5D6F75y = 0;
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
	if (!attack && StopOnEat && moduleMgr->AutoDesync()) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	if (!attack) { Game.getClientInstance()->minecraft->setTimerSpeed(timers); return;	}
	if (nowtick <= 0) attack = false;
	nowtick = nowtick - 1;
	if (!StopOnEat && !moduleMgr->AutoDesync()) Game.getClientInstance()->minecraft->setTimerSpeed(boosttimer);
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);
	delaFADSF7V6AS5DF67ASV5D6F75y++;

	if (delaFADSF7V6AS5DF67ASV5D6F75y >= 5) {
		Vec3 pos = *Game.getLocalPlayer()->getPos();
		C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		pos.y += 0.35f;
		a = C_MovePlayerPacket(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);

		gm->player->velocity.y = 0.465f;
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * boostspeed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * boostspeed;

		gm->player->velocity.x = moveVec.x;
		gm->player->velocity.z = moveVec.z;

		float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

		pos = *gm->player->getPos();
		Game.getLocalPlayer()->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

		gm->player->velocity.y -= 0.15f;
		delaFADSF7V6AS5DF67ASV5D6F75y = 0;
	}
}
void NukeSpeed::onDisable() {
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}