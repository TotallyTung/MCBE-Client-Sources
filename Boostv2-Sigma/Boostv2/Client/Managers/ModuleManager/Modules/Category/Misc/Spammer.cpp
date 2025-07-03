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

	Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(pkt);
}

void Spammer::onNormalTick(LocalPlayer* gm) {
	static int changer = 0;
	static const std::vector<std::string> messages = {
		"Boost activated! Try harder, loser!",
		"You call that skill? Boost is laughing!",
		"Boosting past you like you don't exist!",
		"Oh, did I just BOOST over your tiny ego?",
		"Boost mode ON! Say goodbye to your pride!",
		"Boost: The reason you're losing!",
		"Step aside, weakling. Boost coming through!",
		"Boost is for winners, not crybabies!",
		"Get Boosted or get wrecked!",
		"Boost so fast, you're basically AFK!",
		"Boost: Turning noobs into salt mines!",
		"Boosting circles around your so-called skills!",
		"Is that all you've got? Boost thinks you're pathetic!",
		"Boost up, or quit the game already!",
		"Boosting through your ego like it was never there!"
	};

	if (TimerUtil::hasTimeElapsed((float)delay * 1000)) {
		std::string spamMessage = mode == 1 ? (messages[changer] + " | " + randomString(length)) : messages[changer];
		sendMessage(spamMessage);
		changer = (changer + 1) % messages.size(); // Loop through the messages
	}
}
