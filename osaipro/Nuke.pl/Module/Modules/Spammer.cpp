#include "Spammer.h"
bool debugspam;
bool debugspamsend;
Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay.") {
	registerBoolSetting("SendDebuging", &debugspamsend, false);
	registerBoolSetting("Debuging", &debugspam, false);
	registerIntSetting("Packets", &Packets, Packets, 1, 225);
	registerIntSetting("Delay", &delay, delay, 0, 10);
	registerIntSetting("Length", &length, length, 1, 60);
	registerBoolSetting("Randstr", &bypass, bypass);
	registerBoolSetting("randomPackets", &rp, rp);
	registerIntSetting("Min-Packets", &rpmi, rpmi, 1, 225);
	registerIntSetting("Max-Packets", &rpma, rpma, 1, 225);
	registerBoolSetting("randomLength", &rl, rl);
	registerIntSetting("Min-Length", &rlmi, rlmi, 5, 60);
	registerIntSetting("Max-Length", &rlma, rlma, 5, 60);
	registerBoolSetting("randomDelay", &rd, rd);
	registerIntSetting("Min-Delay", &rdmi, rdmi, 0, 10);
	registerIntSetting("Max-Delay", &rdma, rdma, 0, 10);
}

Spammer::~Spammer() {
}
const char* Spammer::getModuleName() {
	return ("Spammer");
}
void Spammer::onTick(GameMode* gm) {
	Odelay++;
	if (!debugspam) { debugspamsend = false; };
	if (Odelay > delay * 20) {
		for (int i = 0; i < Packets; i++) {
			int randomPackets = Packets;
			int randomLength = length;
			int randomDelay = delay;
			if (rp) {
				randomPackets = random(rpmi, rpma);
			};
			if (rl) {
				randomLength = random(rlmi, rlma);
			};
			if (rd) {
				randomDelay = random(rdmi, rdma);
			};
			if (debugspam) {
				clientMessageF("%sPackets%s: %s%i%s, %sLength%s: %s%i%s, %sDelay%s: %s%i", GOLD, GREEN, BLUE, randomPackets, DARK_PURPLE, GOLD, GREEN, BLUE, randomLength, DARK_PURPLE, GOLD, GREEN, BLUE, randomDelay);
			};
			if (rp) {
				this->Packets = randomPackets;
			}
			if (rl) {
				this->length = randomLength;
			}
			if (rd) {
				this->delay = randomDelay;
			}
			if (debugspam == false || debugspamsend == true && debugspam == true) {
				Utils::sendChat(bypass ? ("#" + Utils::getClientName() + "Client ###Purchase for " + Utils::getClientPrice() + " | " + Utils::randomString(length)) : "#" + Utils::getClientName() + "Client ###Purchase for " + Utils::getClientPrice()); 
			};
			Odelay = 0;
		}
	}
}
