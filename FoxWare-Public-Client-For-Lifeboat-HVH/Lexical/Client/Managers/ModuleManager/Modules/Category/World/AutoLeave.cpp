#include "AutoLeave.h"

AutoLeave::AutoLeave() : Module("AutoLeave", "Each hit is a critical hit", Category::WORLD) {
	registerSetting(new SliderSetting<int>("Minimum Health", "Health At Minimum u want to disconnect at", &health, 10, 1, 19));
}

void LzzMesja(std::string str) {
	std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::Text);
	auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

	pkt->mType = TextPacketType::Chat;
	pkt->mMessage = str;
	pkt->mPlatformId = "";
	pkt->mLocalize = false;
	pkt->mXuid = "";
	pkt->mAuthor = "";

	Game::getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(pkt);
}

void AutoLeave::onNormalTick(LocalPlayer* actor) {
	static float offsetY = 0.0f;

	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr) {
		offsetY = 0.0f;
		return;
	}

	if (Game::getLocalPlayer()->getHealth() <= health) {
		LzzMesja("/hub");
		}
	}