#include "FlyBypass.h"
#include <chrono>
#include "../ModuleManager.h"
FlyBypass::FlyBypass() : IModule(70, Category::MOVEMENT, "2b2e-fly, dev by tony") {
    registerFloatSetting("speed", &speed, speed, 0.f, 0.50f);
    
    enabledTicks = 0;
}

FlyBypass::~FlyBypass() {
}

const char* FlyBypass::getModuleName() {
    return "2B2E-Fly+";
}

void FlyBypass::onEnable() {
    auto player = Game.getLocalPlayer();
    if (!moduleMgr->getModule<NoHurtcam>()->isEnabled()) {
        player->animateHurt(); 
    }
    enabledTicks = 0;
}

void FlyBypass::onTick(GameMode* gm) {
    glideModEffective = glideMod;
    GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

    if (Game.canUseMoveKeys()) {
        if (GameData::isKeyDown(*input->spaceBarKey))
            glideModEffective += 0.0f;
        if (GameData::isKeyDown(*input->sneakKey))
            glideModEffective -= 0.0f;
    }
    gm->player->velocity.y = glideModEffective;
    if (!gm->player->isSprinting() && gm->player->velocity.magnitudexz() > 0.01f) {
        GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
        gm->player->setSprinting(true);
    }

    // Increase the enabled ticks
    enabledTicks += 1;

    // Check if the enabled ticks have exceeded the desired duration (10 ticks = 0.5 seconds)
    if (enabledTicks > 45) {
        this->setEnabled(false);
    }
}

void FlyBypass::onMove(MoveInputHandler* input) {
    auto player = Game.getLocalPlayer();
    if (player == nullptr) return;

    if (player->isInLava() == 1 || player->isInWater() == 1)
        return;

    if (player->isSneaking())
        return;

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
    if (pressed) player->lerpMotion(moveVec);
}

void FlyBypass::onDisable() {
    auto player = Game.getLocalPlayer();
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}
