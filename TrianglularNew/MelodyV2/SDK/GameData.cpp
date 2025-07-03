#include "GameData.h"
bool GameData::isLeftClickDown() {
    return (hidController) ? hidController->leftClickDown : false;
}
bool GameData::isRightClickDown() {
    return (hidController) ? hidController->rightClickDown : false;
}
bool GameData::isKeyDown(int key) {
    static uintptr_t keyMapOffset = 0;
    if (keyMapOffset == 0) {
        const uintptr_t sigOffset = findSig(Sigs::gamedata::isKeyDown);
        if (sigOffset) {
            const int offset = *reinterpret_cast<int*>(sigOffset + 3);
            keyMapOffset = sigOffset - MemoryUtils::getBase() + offset + 7;
        }
    }
    const uintptr_t baseAddress = MemoryUtils::getBase();
    const uintptr_t keyAddress = baseAddress + keyMapOffset + (static_cast<uintptr_t>(key) * 0x4);
    return *reinterpret_cast<bool*>(keyAddress);
}
void GameData::sendKey(int key, bool isDown) {
    using SendKeyFunc = void(__fastcall*)(int, bool);
    static SendKeyFunc sendKeyFunction = reinterpret_cast<SendKeyFunc>(findSig(Sigs::gamedata::sendKey));
    sendKeyFunction(key, isDown);
}
void GameData::DisplayClientMessage(const char* fmt, ...) {
    if (!getLocalPlayer())
        return;

    va_list args;
    va_start(args, fmt);
    const int lengthNeeded = _vscprintf(fmt, args) + 1;
    if (lengthNeeded >= 300) {
        logF("A message that was %i characters long could not be fitted into the buffer", lengthNeeded);
    }
    else {
        char messageBuffer[300];
        vsprintf_s(messageBuffer, 300, fmt, args);
        getLocalPlayer()->displayClientMessage(std::string(messageBuffer));
    }
    va_end(args);
}
GameData mc;