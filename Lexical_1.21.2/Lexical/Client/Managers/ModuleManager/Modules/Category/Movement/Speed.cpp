#include "Speed.h"
#include "../../../../HookManager/Hooks/Input/KeyMapHook.h"
Speed::Speed() : Module("Speed", "Increase Speed", Category::MOVEMENT) {
    registerSetting(new SliderSetting<float>("Value", "How fast we go!", &speed, 1.f, 0.f, 2.f));
    registerSetting(new BoolSetting("Jump", "Jump automatically", &jump, true));
    registerSetting(new EnumSetting("Sprint", "Type of sprint", { "Always", "Smart" }, &sprintType, 0));
}
Vec2<float> getMotion(float speed) {
    float playerYaw = Game::getLocalPlayer()->rotation->prevRot.y;
    bool w = Game::isKeyDown('W');
    bool a = Game::isKeyDown('A');
    bool s = Game::isKeyDown('S');
    bool d = Game::isKeyDown('D');
    if (w) {
        if (!a && !d) playerYaw += 90.0f;
        if (a) playerYaw += 45.0f;
        else if (d) playerYaw += 135.0f;
    }
    else if (s) {
        if (!a && !d) playerYaw -= 90.0f;
        if (a) playerYaw -= 45.0f;
        else if (d) playerYaw -= 135.0f;
    }
    else {
        if (!a && d) playerYaw += 180.0f;
    }
    float calcYaw = playerYaw * 0.01745329251f; //0.01745329251f = PI / 180
    return Vec2<float>(cos(calcYaw) * speed / 10.0f, sin(calcYaw) * speed / 10.0f);
}
bool isMoving(bool jumpCheck) {
    if (!Game::canUseMoveKeys()) return false;
    if (Game::isKeyDown('W') || Game::isKeyDown('A') || Game::isKeyDown('S') || Game::isKeyDown('D')) return true;
    if (jumpCheck && Game::isKeyDown(VK_SPACE)) return true;
    return false;
}

void setSpeed(float speed) {
    Vec2<float> motion = getMotion(speed);
    if (!isMoving(false)) {
        motion.x = 0.f;
        motion.y = 0.f;
    }
    Game::getLocalPlayer()->stateVector->velocity.x = motion.x;
    Game::getLocalPlayer()->stateVector->velocity.z = motion.y;
}


void Speed::onNormalTick(LocalPlayer* actor) {
	float realSpeed = speed * 10.f;
	if (sprintType == 0) Game::getLocalPlayer()->setSprinting(true);
	if (sprintType == 1 && Game::getLocalPlayer()->getItemUseDuration() <= 0) Game::getLocalPlayer()->setSprinting(true);
	else if (sprintType == 1 && Game::getLocalPlayer()->getItemUseDuration() > 0) Game::getLocalPlayer()->setSprinting(false);
	if (jump) {
		if (isMoving && Game::canUseMoveKeys()) KeyMapHook::createKeyPress(VK_SPACE, true);
		else if (!isMoving || !Game::canUseMoveKeys()) KeyMapHook::createKeyPress(VK_SPACE, false);
	}
	setSpeed(realSpeed); // bypasses soon
}