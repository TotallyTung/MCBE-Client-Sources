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
		"Foxy Client activated! Let's make this fun!",
		"Oh wow, Foxy Client just made that easy! Keep going!",
		"Look at that, Foxy Client is here to help you shine!",
		"Foxy Client: Helping you level up, one step at a time!",
		"Boost mode ON with Foxy Client! Time to show your true potential!",
		"With Foxy Client, you're unstoppable – let's go!",
		"Foxy Client coming through to make this a breeze!",
		"Foxy Client is for champions! Keep up the great work!",
		"Get Boosted with Foxy Client – victory is just around the corner!",
		"Boost so smooth with Foxy Client, it's like flying!",
		"Foxy Client: Turning challenges into opportunities!",
		"Boosting your way to the top with Foxy Client – you're doing great!",
		"Foxy's got your back – let's take this to the next level!",
		"Boost up with Foxy Client – you've got this!",
		"Foxy Client: Elevating your game with every move!"
	};

	if (TimerUtil::hasTimeElapsed((float)delay * 1000)) {
		std::string spamMessage = mode == 1 ? (messages[changer] + " | " + randomString(length)) : messages[changer];
		sendMessage(spamMessage);
		changer = (changer + 1) % messages.size(); // Loop through the messages
	}
}
