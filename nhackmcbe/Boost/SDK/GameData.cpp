#include "GameData.h"

GameData mc;

bool GameData::isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (!keyMapOffset) {
		uintptr_t sigOffset = findSig(Sigs::gamedata::isKeyDown);
		if (sigOffset) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));
			keyMapOffset = sigOffset - MemoryUtils::getBase() + offset + 7;
		}
	}
	return *reinterpret_cast<bool*>(MemoryUtils::getBase() + keyMapOffset + ((uintptr_t)key * 0x4));
}

void GameData::sendKey(int key, bool isDown) {
	using sendKey_t = void(__fastcall*)(int, bool);
	static sendKey_t sendKeyFunc = (sendKey_t)findSig(Sigs::gamedata::sendKey);
	sendKeyFunc(key, isDown);
}

bool GameData::isLeftClickDown() {
	if (hidController == nullptr)
		return false;
	return hidController->leftClickDown;
}

bool GameData::isRightClickDown() {
	if (hidController == nullptr)
		return false;
	return hidController->rightClickDown;
}

void GameData::DisplayClientMessage(const char* fmt, ...) {
	if (getLocalPlayer() == nullptr) return;

	va_list arg;
	va_start(arg, fmt);
	auto lengthNeeded = _vscprintf(fmt, arg) + 1;
	if (lengthNeeded >= 300) {
		logF("A message that was %i characters long could not be fitted into the buffer", lengthNeeded);
	}
	else {
		char message[300];
		vsprintf_s(message, 300, fmt, arg);
		std::string msg(message);
		getLocalPlayer()->displayClientMessage(msg);
	}
	va_end(arg);
}