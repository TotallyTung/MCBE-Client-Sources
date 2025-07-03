#include "SwingSpeed.h"

SlowSwing::SlowSwing() : IModule(0x0, Category::VISUAL, "Slows down swing speed") {
	registerFloatSetting("Speed", &swingspeed, swingspeed, 1.f, 20.f);
}

SlowSwing::~SlowSwing() {
}

const char* SlowSwing::getModuleName() {
	return ("SlowSwing");
}

void* SmoothSwing2 = (void*)FindSignature("0F 84 ? ? ? ? 48 8B 56 ? 48 85 D2 74 ? 48 8B 02");

void SlowSwing::onTick(GameMode* gm) {
	if (useSwong) SwingSpeedIndex += swingspeed;
}

void SlowSwing::onPlayerTick(Player* plr) {
	static unsigned int offset = *reinterpret_cast<int*>(FindSignature("80 BB ? ? ? ? 00 74 1A FF 83") + 2);
	float* stopswing = reinterpret_cast<float*>((uintptr_t)Game.getLocalPlayer() + offset);
	*stopswing = 0;
	if (SwingSpeedIndex >= 100) {
		useSwong = false;
		SwingSpeedIndex = 0;
	}
	if (useSwong) {
		std::vector<float> SwingArray;
		for (int i = 0; i < 100; i++)
			SwingArray.push_back(i * 0.01);
		if (SwingSpeedIndex <= 100) {
			float SwingSpeedArray = SwingArray[SwingSpeedIndex];
			float* speedAdr = reinterpret_cast<float*>(reinterpret_cast<__int64>(Game.getLocalPlayer()) + 0x79C);
			*speedAdr = SwingSpeedArray;
		}
	}
	else {
		SwingSpeedIndex = 0;
	}
}
void SlowSwing::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<AnimatePacket>()) {
		useSwong = true;
	}
}