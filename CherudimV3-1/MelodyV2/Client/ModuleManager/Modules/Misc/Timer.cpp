#include "Timer.h"

Timer::Timer() : Module("Timer", "Changes your tps(tick per seconds).", Category::MISC) {
	addSlider<int>("TPS", "NULL", ValueType::INT_T, &tps, 1, 150);
}

void Timer::onNormalTick(Actor* actor) {
	if (mc.getLocalPlayer() == nullptr) return;

	mc.getClientInstance()->minecraft->mctimer->tps = (float)tps;
}

void Timer::onDisable() {
	mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
}
