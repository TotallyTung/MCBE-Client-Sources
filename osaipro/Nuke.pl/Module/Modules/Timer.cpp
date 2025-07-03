#include "Timer.h"
#include "../ModuleManager.h"

Timer::Timer() : IModule(0, Category::PLAYER, "Increase the ticks per second.") {
	registerIntSetting("TPS", &timer, 24, 0, 500);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer1");
}

void Timer::onTick(GameMode* gm) {
	if (moduleMgr->AutoDesync(true)) return;
	Game.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Timer::onDisable() {
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}