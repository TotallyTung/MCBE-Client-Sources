#include "ReverseStep.h"

ReverseStep::ReverseStep() : IModule(0x0, Category::MOVEMENT, "Go down blocks faster (CPVP PLAYERS LOVE THIS!!!)") {
}

ReverseStep::~ReverseStep() {
}

const char* ReverseStep::getModuleName() {
	return ("ReverseStep");
}

void ReverseStep::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (gm->player->onGround && !gm->player->isInWater() && !gm->player->isInLava()) {
		gm->player->velocity.y = -1;
	}//Totally not the code from phobos :D 
	 //Totally not the code from badman :D - JohnTWD
}