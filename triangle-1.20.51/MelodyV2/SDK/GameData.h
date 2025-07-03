#pragma once
#include "Classes/ClientInstance.h"
#include "Classes/LocalPlayer.h"
#include "Classes/HIDController.h"
#include "MCTextFormat.h"

class GameData {
private:
	ClientInstance* clientInstance = nullptr;
	HIDController* hidController = nullptr;
public:
	inline ClientInstance* getClientInstance() { return this->clientInstance; }
	inline void setClientInstance(ClientInstance* ci) { this->clientInstance = ci; }
	inline HIDController* getHIDController() { return this->hidController; }
	inline void setHIDController(HIDController* Hid) { this->hidController = Hid; }
	inline LocalPlayer* getLocalPlayer() {
		if (clientInstance == nullptr)
			return 0;
		else
		return clientInstance->getLocalPlayer();
	}
	inline GameMode* getGameMode() {
		//if (getLocalPlayer() == nullptr) return nullptr;
		return getLocalPlayer()->getGameMode();
	}
	inline void sendPacket(Packet* packet) {
		if (clientInstance->loopbackPacketSender != nullptr) clientInstance->loopbackPacketSender->send(packet);
	}
	inline bool canUseMoveKeys() {
		if (clientInstance->minecraftGame == nullptr) return false;
		return clientInstance->minecraftGame->canUseKeys;
	}
public:
	int cameraPerspectiveMode = 0;
	bool isKeyDown(int key);
	void sendKey(int key, bool isDown);
	bool isLeftClickDown();
	bool isRightClickDown();
	void DisplayClientMessage(const char* fmt, ...);
};

extern GameData mc;