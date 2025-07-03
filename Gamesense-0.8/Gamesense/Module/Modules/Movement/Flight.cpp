#include "Flight.h"
#include <cmath>

bool alground = false;
float hiveflytim = 1;
uintptr_t ViewBobbing = FindSignature("0F B6 80 DB 01 00 00");

using namespace std;
Flight::Flight() : IModule(0, Category::MOVEMENT, "Allows you to fly") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Airwalk", 1);
	registerBoolSetting("Clip Up", &clip, clip);
	registerBoolSetting("ViewBobbing", &viewBobbing, viewBobbing);
	registerBoolSetting("Damage", &damage, damage);
	registerBoolSetting("Boost", &boost, boost);
	registerFloatSetting("Clip Height", &clipHeight, clipHeight, -3.f, 4.f);
	registerFloatSetting("Duration", &duration, duration, 0.5f, 1.05f);
	registerFloatSetting("Speed", &speed, speed, 0.2f, 15.f);
	registerFloatSetting("Glide", &value, value, -0.15f, 0.00f);
}

const char* Flight::getRawModuleName() {
	return "Flight";
}

const char* Flight::getModName() {
	switch (mode.getSelectedValue()) {
	case 0:
		name = " Vanilla";
		break;
	case 1:
		name = " Airwalk";
		break;
	}
	return name;
}

void Flight::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	FallVelocity = HiveVelocity;
	falldist = 0;
	enabledpos = player->currentPos;

	alground = false;
		if (clip) {
			vec3_t myPos = *player->getPos();
			myPos.y += clipHeight;
			player->setPos(myPos);
		}
	// Variables
	auto auraMod = moduleMgr->getModule<Killaura>();
	//auto disablerMod = moduleMgr->getModule<Disabler>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto blinkMod = moduleMgr->getModule<Blink>();
	if (speedMod->isEnabled()) speedWasEnabled = true;
	PointingStruct* level = player->pointingStruct;
	vec3_t pos2 = *player->getPos();
	vec3_t* pos = player->getPos();
	oldpos.x = player->getPos()->x;
	oldpos.y = player->getPos()->y;
	oldpos.z = player->getPos()->z;
	viewBobbingEffective = false;
	tick = 0;
	dash = false;
	if (damage) { level->playSound("game.player.hurt", *pos, 1, 1); player->animateHurt(); }
	if (boost) speedDuration = speed;
}

bool Flight::isHoldMode() {
	return mode.getSelectedValue() == 2;
}

void Flight::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	tick++;

	// Math
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (mode.getSelectedValue() != 5)
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;

	auto speedMod = moduleMgr->getModule<Speed>();
	auto blinkMod = moduleMgr->getModule<Blink>();
	auto blink = moduleMgr->getModule<Blink>();

	float yaw = player->yaw; float pitch = player->pitch;
	float velocity = g_Data.getLocalPlayer()->velocity.magnitude();
	float velocityxz = g_Data.getLocalPlayer()->velocity.magnitudexz();
	float velocityxy = g_Data.getLocalPlayer()->velocity.magnitudexy();
	bool jumping = GameData::isKeyDown(*input->spaceBarKey);
	bool sneaking = GameData::isKeyDown(*input->sneakKey);
	isUp = false;
	isDown = false;
	isUp = jumping;
	isDown = sneaking;
	vec3_t* pos = g_Data.getLocalPlayer()->getPos();
	float calcYaw = (yaw + 90) * (PI / 180);
	float calcPitch = (pitch) * -(PI / 180);
	vec3_t moveVec;

	if (speedMod->isEnabled()) {
		//auto notification = g_Data.addNotification("Flight:", "Disabled Speed"); notification->duration = 5;
		speedMod->setEnabled(false);
	}

	// Vanilla
	if (mode.getSelectedValue() == 0) {
		if (g_Data.canUseMoveKeys()) {
			if (jumping)
				effectiveValue += currSpeed;
			if (sneaking)
				effectiveValue -= currSpeed;
		}
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) MoveUtil::fullStop(false);
		gm->player->velocity.y = effectiveValue;
		effectiveValue = value;
	}

	// Airwalk
	if (mode.getSelectedValue() == 1) {
		if (speedMod->isEnabled() || jumping) player->onGround = false;
		else player->onGround = true;
		gm->player->velocity.y = effectiveValue;
		effectiveValue = value;
	}
}

void Flight::onPlayerTick(C_Player* plr) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void Flight::onMove(C_MoveInputHandler* input) {
	auto mblink = moduleMgr->getModule<Blink>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	bool pressed = MoveUtil::isMoving();
	vec3_t pos = *player->getPos();

	if (boost) {
		if (speedDuration >= 0.158f) speedDuration *= duration;
		currSpeed = speedDuration;

		if (!viewBobbingEffective) {
			if (player->onGround && pressed) player->jumpFromGround();
			viewBobbingEffective = true;
		}
	}
	else { viewBobbingEffective = true; currSpeed = speed; }

	if (viewBobbing) {
		if (viewBobbingEffective) Utils::patchBytes((unsigned char*)ViewBobbing, (unsigned char*)"\xB8\x01\x00\x00\x00\x90\x90", 7);
		if (effectiveValue >= 0.00) effectiveValue = -0.0000000001;
	}
	else Utils::patchBytes((unsigned char*)ViewBobbing, (unsigned char*)"\x0F\xB6\x80\xDB\x01\x00\x00", 7);

	// Vanilla, Jump
	if (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 3 || mode.getSelectedValue() == 4 || mode.getSelectedValue() == 9 || mode.getSelectedValue() == 10)
		MoveUtil::setSpeed(currSpeed);

	float calcYaw = (player->yaw + 90) * (PI / 180);
	float c = cos(calcYaw);
	float s = sin(calcYaw);

	vec2_t moveVec2D = { input->forwardMovement, -input->sideMovement };
	moveVec2D = { moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c };
	vec3_t moveVec;
}

void Flight::onSendPacket(C_Packet* packet) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	vec3_t pos;
	pos.x = player->getPos()->x;
	pos.y = player->getPos()->y;
	pos.z = player->getPos()->z;
	static int tick = 1; tick++;

	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		if (mode.getSelectedValue() == 4) {
			movePacket->onGround = true;
		}

		if (mode.getSelectedValue() == 10) {
			movePacket->onGround = false;
		}
	}

	if (packet->isInstanceOf<NetworkLatencyPacket>()) {
		auto* LatencyPacket = reinterpret_cast<NetworkLatencyPacket*>(packet);
	}
	oldpos.x = player->getPos()->x;
	oldpos.y = player->getPos()->y;
	oldpos.z = player->getPos()->z;
}

void Flight::onDisable() {
	Utils::patchBytes((unsigned char*)ViewBobbing, (unsigned char*)"\x0F\xB6\x80\xDB\x01\x00\x00", 7);
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
	//auto disablerMod = moduleMgr->getModule<Disabler>();
	auto auraMod = moduleMgr->getModule<Killaura>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto blink = moduleMgr->getModule<Blink>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	player->velocity.x = 0.f;
	player->velocity.z = 0.f;

	//if (disablerWasEnabled) { disablerMod->setEnabled(true); disablerWasEnabled = false; }
	if (speedWasEnabled) { speedMod->setEnabled(true); speedWasEnabled = false; }
	if (viewBobbing) player->onGround = false;
	velocityEffective = false;
	blink->setEnabled(false);

	hiveSpeedIndex = 0;
	hiveVelocity = 0;
	enabledTick = 0;
	hiveTick = 0;

	// Jump
	if (mode.getSelectedValue() == 3 || mode.getSelectedValue() == 10) hasJumped = false;
}
