#include "Timer1.h"

Timer1::Timer1() : IModule(0, Category::PLAYER, "Modifies the games speed") {
	registerIntSetting("TPS", &timer2, timer2, 20, 30);
}

const char* Timer1::getModuleName() {
	return ("TimerRestricted");
}

void Timer1::onTick(GameMode* gm) {
	if (moduleMgr->AutoDesync(true)) return;
	Game.getClientInstance()->minecraft->setTimerSpeed(timer2);
}

void Timer1::onDisable() {
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}