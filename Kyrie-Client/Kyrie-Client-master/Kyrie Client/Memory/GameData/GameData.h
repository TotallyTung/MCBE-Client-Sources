#pragma once
#include "../../SDK/ClientInstance.h"
#include "../../SDK/Player.h"

class GameData {
private:
	ClientInstance* clientInstance = nullptr;
	Player* localPlayer = nullptr;
public:
	inline ClientInstance* getClientInstance() { return this->clientInstance; }
	inline void setClientInstance(ClientInstance* ci) { this->clientInstance = ci; }
	inline Player* getLocalPlayer() {
		if (this->clientInstance == nullptr) return nullptr;
		return this->clientInstance->getLocalPlayer();
	}
public:
	// ClickGui stuff
	bool isLeftClickDown = false;
	bool isRightClickDown = false;
	bool isHoldingLeftClick = false;
	bool isHoldingRightClick = false;
};

extern GameData mc;