#include "Disabler.h"
#include <Windows.h> 

Disabler::Disabler() : Module("Disabler", "Disable the anticheat", Category::MISC) {
    registerSetting(new EnumSetting("Server", "NULL", { "Lifeboat", "Guyser" }, &Mode, 1));
}

std::string Disabler::getModeText() {
    if (Mode == 0) {
        return "Lifeboat";
    }
    else if (Mode == 1) {
        return "Guyser";
    }
    return "Unknown";
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

void Disabler::onSendPacket(Packet* packet) {

    /* lifeboat*/
    if (Mode == 0 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        paip->mPlayMode = ClientPlayMode::Screen;
        paip->mInputMode = InputModeAuth::Touch;
        paip->TicksAlive = 0;
        if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
            hitResult->type = HitResultType::AIR;
        }
    }

    /* guyser */
    if (Mode == 1 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        if (paip) {
            if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
                hitResult->type = HitResultType::AIR;
            }
        }
        if (mpp) {
            if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
                hitResult->type = HitResultType::AIR;
            }
        }
    }

}
