#include "Timer.h"

Timer::Timer() : Module("Timer", "Modify TPS (ticks per second) of game", Category::MISC) {
	registerSetting(new SliderSetting<int>("TPS", "ticks per second", &timerValue, 40, 0, 200));
}

void Timer::onDisable() {
	Minecraft* mc = Game::clientInstance->minecraft;
	if (mc != nullptr) {
		*mc->minecraftTimer = 20.0f;
		*mc->minecraftRenderTimer = 20.0f;
	}
}

void Timer::onClientTick() {
	Minecraft* mc = Game::clientInstance->minecraft;
	if (mc != nullptr) {
		*mc->minecraftTimer = (float)timerValue;
		*mc->minecraftRenderTimer = (float)timerValue;
	}
}