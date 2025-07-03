#include "FastStop.h"

FastStop::FastStop() : IModule(0, Category::MOVEMENT, "Stops fast yes") {
}

const char* FastStop::getRawModuleName() {
	return ("FastStop");
}

void FastStop::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	bool pressed = MoveUtil::keyPressed();
	if (!pressed) MoveUtil::stop(true);
}