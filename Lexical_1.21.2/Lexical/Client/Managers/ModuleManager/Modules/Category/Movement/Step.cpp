#include "Step.h"

Step::Step() : Module("Step", "Stepping on blocks", Category::MOVEMENT) {
	registerSetting(new SliderSetting<int>("Step value", "NULL", &value, 2, 2, 5));
}

void Step::onNormalTick(LocalPlayer* localPlayer) {
	Game::getLocalPlayer()->getComponent<MaxAutoStepComponent>()->maxAutoStep = value;
}

void Step::onDisable() {
	if (Game::getLocalPlayer() != nullptr)
		Game::getLocalPlayer()->getComponent<MaxAutoStepComponent>()->maxAutoStep = 0.5625f;
}