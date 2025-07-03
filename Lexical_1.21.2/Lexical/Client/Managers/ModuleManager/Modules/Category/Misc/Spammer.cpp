#include "Spammer.h"
int length = 8;
Spammer::Spammer() : Module("Spammer", "Spams a message in a specified delay", Category::MISC) {
	registerSetting(new SliderSetting<int>("Delay", "Delay in seconds between messages", &delay, 1, 1, 20));
	registerSetting(new SliderSetting<int>("Length", "Random string length for bypass", &length, 1, 1, 20));
	registerSetting(new EnumSetting("Mode", "Spamming mode", { "Normal", "Bypass" }, &mode, 0));
}

Spammer::~Spammer() {
}

std::string Spammer::getModeText() {
	if (mode == 1)
		return "Bypass";
	else
		return "Normal";
}

static inline std::string randomString(const int size) {
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(str.begin(), str.end(), generator);
	return str.substr(0, size);
}

void sendMessage(std::string str) {
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

void Spammer::onNormalTick(LocalPlayer* gm) {
	static int changer = 0;
	static const std::vector<std::string> messages = {
		"Get the best experience with Lexical Client for 1.21.2!",
		"Lexical Client - top choice for 2b2tpe and 2b2tmcpe players!",
		"Dominate IGNPvP with Lexical Client’s advanced features!",
		"Lexical Client is the ultimate client for CPE and 2b2e servers!",
		"Want smooth gameplay on 1.21.2? Use Lexical Client now!",
		"Join the pros playing with Lexical Client on 2b2tpe and beyond!",
		"Lexical Client: optimized for 2b2tmcpe, IGNPvP, and more!",
		"Upgrade your Minecraft 1.21.2 with Lexical Client today!",
		"Lexical Client — your edge on 2b2e and competitive servers!",
		"Don’t get left behind, switch to Lexical Client for best results!",
	};

	if (TimerUtil::hasTimeElapsed((float)delay * 1000)) {
		std::string spamMessage = mode == 1 ? (messages[changer] + " | " + randomString(length)) : messages[changer];
		sendMessage(spamMessage);
		changer = (changer + 1) % messages.size();
	}
}
