#include "ReverseStep.h"

ReverseStep::ReverseStep() : IModule(0x0, Category::MOVEMENT, "Falls down blocks faster")
{
	registerBoolSetting("Jump", &jump, jump, "Always fall fast even when jumping");
}

ReverseStep::~ReverseStep()
{
}

const char *ReverseStep::getRawModuleName()
{
	return ("FastFall");
}

void ReverseStep::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	if (!jump && gm->player->onGround && !gm->player->isInWater() && !gm->player->isInLava())
	{
		gm->player->velocity.y = -1;
	}
	if (jump && !gm->player->isInWater() && !gm->player->isInLava())
	{
		gm->player->velocity.y = -1;
	}
}