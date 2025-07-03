#include "Spammer.h"

Spammer::Spammer() : IModule(0, Category::EXPLOIT, "Automatically spams stuff")
{
	registerBoolSetting("Bypass", &bypass, bypass);
	registerIntSetting("BP-Length", &length, length, 1, 10);
	messageMode = (*new SettingEnum()).addEntry(EnumEntry("Advertise", 0)).addEntry(EnumEntry("Toxic", 1));
	registerEnumSetting("Mode", &messageMode, 0);
	registerIntSetting("Delay", &delay, delay, 1, 20);
}
Spammer::~Spammer()
{
}

const char *Spammer::getRawModuleName()
{
	return "Spammer";
}

void Spammer::onEnable()
{
	messageIndex = 0;
}

void Spammer::onTick(C_GameMode *gm)
{
	C_TextPacket textPacket;
	messageTimer++;
	if (messageIndex <= 0)
		messageIndex = 1; // Prevent invis msgs
	switch (messageMode.selected)
	{
	case 0:
		if (messageIndex > 13)
			messageIndex = 0;
		if (messageTimer >= delay * 20)
		{
			for (int i = 0; i < messageIndex; i++)
				textPacket.message.setText(bypass ? (Utils::randomString(length) + " | " + LoholismList[i] + " | " + Utils::randomString(length)) : LoholismList[i]);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			messageIndex++;
			messageTimer = 0;
		}
		break;
	case 1:
		if (messageIndex > 15)
			messageIndex = 0;
		if (messageTimer >= delay * 20)
		{
			for (int i = 0; i < messageIndex; i++)
				textPacket.message.setText(bypass ? (Utils::randomString(length) + " | " + toxicList[i] + " | " + Utils::randomString(length)) : toxicList[i]);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			messageIndex++;
			messageTimer = 0;
		}
		break;
	}
}
