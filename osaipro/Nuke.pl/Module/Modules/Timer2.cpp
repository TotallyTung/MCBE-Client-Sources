#include "Timer2.h"

Timer2::Timer2() : IModule(0, Category::PLAYER, "Modifies the game's speed") {
    registerIntSetting("TPS", &timer3, timer3, 1, 500);
}

const char* Timer2::getModuleName() {
    return "TimerFlashBypass";
}

void Timer2::onTick(GameMode* gm) {
    static int tickCounter = 0; 
    if (moduleMgr->AutoDesync(true)) return;
    auto player = Game.getLocalPlayer();
    Game.getClientInstance()->minecraft->setTimerSpeed(timer3);
    tickCounter++;
    if (tickCounter >= 13) {
        this->setEnabled(false); 
        tickCounter = 0; 
    }
}

void Timer2::onDisable() {
    Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}
