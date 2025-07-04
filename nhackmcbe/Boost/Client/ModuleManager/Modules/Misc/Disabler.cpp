#include "Disabler.h"
#include <Windows.h> // for Windows time functions

Disabler::Disabler() : Module("Disabler", "Disable the anticheat", Category::MISC) {
    addEnumSetting("Server", "What server do you want this to work on", { "Lifeboat" }, &Mode);
}
std::string Disabler::getModName() {
    static char textStr[15];
    sprintf_s(textStr, 15, "Lifeboat");
    return std::string(textStr);
}
float lerp(float endPoint, float current, float speed) {
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = std::max(endPoint, current) - std::min(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}
static __int64 ms;
static DWORD lastMS = GetTickCount();
static __int64 timeMS = -1;
static DWORD getCurrentMs() {
    return GetTickCount();
}
static __int64 getElapsedTime() {
    return getCurrentMs() - ms;
}
static void resetTime() {
    lastMS = getCurrentMs();
    timeMS = getCurrentMs();
}
static bool hasTimedElapsed(__int64 time, bool reset) {
    if (getCurrentMs() - lastMS > time) {
        if (reset)
            resetTime();
        return true;
    }
    return false;
}
void Disabler::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (Mode == 0 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;

        if (paip) {
            float perc = static_cast<float>(paip->ticksAlive % (12 * 2)) / (12 * 2.0f);
            paip->position.y = lerp(paip->position.y, paip->position.y - 0.2f, perc);
            paip->downVelocity = -(1.0f / (12 * 2.0f));
            if (paip->ticksAlive % 12 == 0 || paip->ticksAlive % 12 == 12) {
                paip->inputData |= AuthInputAction::START_JUMPING;
            }
            paip->inputData |= AuthInputAction::JUMPING;
        }

        if (mpp) {
            float perc = static_cast<float>(mpp->tick % 24) / 24.0f;
            mpp->position.y = lerp(mpp->position.y, mpp->position.y - 0.2f, perc);
            mpp->onGround = true;
        }
    }
}

