#include "AntiCrystal.h"
using namespace std;
AntiCrystal::AntiCrystal() : IModule(0x0, Category::PLAYER, "Silently phases ur bottom half")
{
	registerBoolSetting("Height", &Bypass, Bypass, "Control height");
	registerBoolSetting("Geyser", &endzone, endzone, "Makes it work for servers that use geyser like endzone");
	registerFloatSetting("Height", &height, height, 0.f, 1.60f, 0.1f, "The height of ur bottom half to phase");
}

AntiCrystal::~AntiCrystal()
{
}
const char *AntiCrystal::getRawModuleName()
{
	return ("AntiCrystal");
}
const char* AntiCrystal::getModName()
{
	if (height)
		name = " Height";
	else if (!height)
		name = " Normal";
	return name;
}
void AntiCrystal::onSendPacket(C_Packet *packet)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	vec3_t *pos = g_Data.getLocalPlayer()->getPos();
	if (!Bypass && !endzone)
	{
		if (packet->isInstanceOf<PlayerAuthInputPacket>())
		{
			PlayerAuthInputPacket *InputPacket = reinterpret_cast<PlayerAuthInputPacket *>(packet);
			InputPacket->pos.y = static_cast<float>(pos->y - 1);
		}
		else if (packet->isInstanceOf<C_MovePlayerPacket>())
		{
			C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
			movePacket->Position.y = static_cast<float>(pos->y - 1);
		}
	}
	if (Bypass)
	{
		if (packet->isInstanceOf<PlayerAuthInputPacket>())
		{
			PlayerAuthInputPacket *InputPacket = reinterpret_cast<PlayerAuthInputPacket *>(packet);
			InputPacket->pos.y = static_cast<float>(pos->y - height);
		}
		else if (packet->isInstanceOf<C_MovePlayerPacket>())
		{
			C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
			movePacket->Position.y = static_cast<float>(pos->y - height);
		}
	}
	if (endzone)
	{
		if (packet->isInstanceOf<PlayerAuthInputPacket>())
		{
			PlayerAuthInputPacket *InputPacket = reinterpret_cast<PlayerAuthInputPacket *>(packet);
			if (timer = 1)
			{
				InputPacket->pos.y = static_cast<float>(pos->y - 0.1);
			}
			if (timer = 2)
			{
				InputPacket->pos.y = static_cast<float>(pos->y - 0.3);
			}
			if (timer = 3)
			{
				InputPacket->pos.y = static_cast<float>(pos->y - 0.1);
			}
			if (timer = 4)
			{
				InputPacket->pos.y = static_cast<float>(pos->y - 0.6);
			}
			if (timer = 5)
			{
				timer = 0.f;
			}
		}
		else if (packet->isInstanceOf<C_MovePlayerPacket>())
		{
			C_MovePlayerPacket *movePacket = reinterpret_cast<C_MovePlayerPacket *>(packet);
			movePacket->Position.y = static_cast<float>(pos->y - height);
		}
	}
}