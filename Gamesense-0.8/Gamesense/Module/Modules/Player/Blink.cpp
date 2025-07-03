#include "Blink.h"

using namespace std;
Blink::Blink() : IModule(0, Category::PLAYER, "Lags you server-sided")
{
	registerBoolSetting("Trail", &trail, trail);
	registerBoolSetting("Hold", &hold, hold);
}

Blink::~Blink()
{
	getMovePlayerPacketHolder()->clear();
	getPlayerAuthInputPacketHolder()->clear();
}

const char *Blink::getRawModuleName()
{
	return ("Blink");
}
const char *Blink::getModuleName()
{
	if (hold)
		name = string("Blink ") + string(GRAY) + string("Hold");
	else
		return "Blink";
	return name.c_str();
}
bool Blink::isHoldMode()
{
	if (hold)
		return true;
	else
	{
		return false;
	}
}
void Blink::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	player->fallDistance = 0.f;

	vec3_t eyePos = player->eyePos0;
	eyePos.y = eyePos.y - 1.5f;
	blockBelow.push_back(eyePos);
}
void Blink::onLevelRender()
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	auto i = ColorUtil::interfaceColor(1);

	DrawUtils::setColor(i.r, i.g, i.b, 1);
	if (trail)
	{
		DrawUtils::drawLineStrip3D(blockBelow, 5);
	}
}
void Blink::onDisable()
{
	blockBelow.clear();
}