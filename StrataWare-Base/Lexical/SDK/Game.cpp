#include "Game.h"

ClientInstance* Game::clientInstance = nullptr;
void* Game::keyMapPtr = nullptr;
void* Game::keyMousePtr = nullptr;
int Game::viewPerspectiveMode = 0;

bool Game::isKeyDown(uint32_t key) {
	if (keyMapPtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMapPtr + ((uintptr_t)key * 0x4));
}

bool Game::isLeftClickDown() {
	if (keyMousePtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMousePtr + 1);
}

bool Game::isRightClickDown() {
	if (keyMousePtr == nullptr) {
		return false;
	}

	return *reinterpret_cast<bool*>((uintptr_t)keyMousePtr + 2);
}

void Game::DisplayClientMessage(const char* fmt, ...) {
	LocalPlayer* localPlayer = Game::ourPlayer();
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