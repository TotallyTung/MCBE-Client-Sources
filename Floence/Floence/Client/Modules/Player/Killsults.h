#pragma once

class Killsults : public Module
{
private:
	bool notification = true;
	bool sound = true;
	int mode = 0;

	bool mentionplayer = true;

public:
	Killsults(int keybind = Keys::NONE, bool enabled = false) :
		Module("Killsults", "Player", "Insults the people you have killed", keybind, enabled)
	{
		addEnum("Mode", "The mode of message", { "Normal" }, &mode);
		addBool("Notification", "Show kill notification", &notification);
		addBool("Sound", "Play kill sound", &sound);
		addBool("Mention Player", "Mention the player you kill in a @target", &mentionplayer);
	}

	std::string Normal[3] = {
		"!target!,did you know?Valkyrie is best of survival guide.everybody can't survival 7day without this!",
		"i manage you,!target!.",
		"your client is suck.you should stop,!target!",
	};


	void sendMessage(std::string str) {
		std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(9);
		auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

		pkt->type = TextPacketType::CHAT;
		pkt->message = str;
		pkt->PlatformId = "";
		pkt->Localize = false;
		pkt->Xuid = "";
		pkt->author = "";

		Address::getClientInstance()->getLoopbackPacketSender()->sendToServer(pkt);
	}

	std::string getKillsult(std::string const& target) {
		int randomVal = 0;
		randomVal = rand() % 3;

		std::string insult = Normal[randomVal];

		if (insult.find("!target!") != std::string::npos) {
			insult.replace(insult.find("!target!"), 8, "@" + target);
		}

		return insult;
	}

	void onEvent(ActorBaseTickEvent* event) override {
		auto player = Address::getLocalPlayer();
		if (player == nullptr) return;

		if (Global::Killed) {
			sendMessage(getKillsult(Global::KilledTarget));
		if (sound)	{
			//PlaySound
			}

			Global::Killed = false;
		}
	}
};