#include "Spammer.h"

Spammer::Spammer() : IModule(0, Category::EXPLOIT, "Automatically spams stuff")
{
	registerBoolSetting("Bypass", &bypass, bypass);
	registerIntSetting("BP-Length", &length, length, 1, 10);
	messageMode = SettingEnum(this)
		.addEntry("Advertise", 0)
		.addEntry("Toxic", 1);
	registerEnumSetting("Mode", &messageMode, 0);
	registerIntSetting("Delay", &delay, delay, 0, 150);
}
Spammer::~Spammer()
{
}

const char* Spammer::getModuleName()
{
	return "Spammer";
}

void Spammer::onEnable()
{
	messageIndex = 0;
}

void Spammer::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (messageTimer < delay) {
		messageTimer++;
		return;
	}
	C_TextPacket textPacket;
	switch (messageMode.getSelectedValue()) {
	case 0:
		textPacket.message.setText(bypass ? (Utils::randomString(length) + " | " + LoholismList[messageIndex] + " | " + Utils::randomString(length)) : LoholismList[messageIndex]);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		messageIndex++;
		if (messageIndex >= LoholismList.size())
			messageIndex = 0;
		break;
	case 1:
		textPacket.message.setText(bypass ? (Utils::randomString(length) + " | " + toxicList[messageIndex] + " | " + Utils::randomString(length)) : toxicList[messageIndex]);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		messageIndex++;
		if (messageIndex >= toxicList.size())
			messageIndex = 0;
		break;
	}
	messageTimer = 0;
}
