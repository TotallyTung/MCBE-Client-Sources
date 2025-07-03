#include "Game.h"
GameData Game;


bool GameData::isKeyDown(uint32_t key) {
	if (keyMapPtr == nullptr) {
		return false;
	}
	
	return *reinterpret_cast<bool*>((uintptr_t)keyMapPtr + ((uintptr_t)key * 0x4));
}

bool GameData::isLeftClickDown() {
	if (keyMousePtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMousePtr + 1);
}

bool GameData::isRightClickDown() {
	if (keyMousePtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMousePtr + 2);
}
bool GameData::isWheelDown() {
	if (keyMousePtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMousePtr + 3);
}

void GameData::DisplayClientMessage(const char* fmt, ...) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	va_list arg;
	va_start(arg, fmt);
	char message[300];
	vsprintf_s(message, 300, fmt, arg);
	va_end(arg);

	std::string messageStr(message);
	localPlayer->displayClientMessage(messageStr);
}