#include "NoSlowDown.h"

NoSlowDown::NoSlowDown() : IModule(0, Category::MOVEMENT, "Don't get slowed down when blocking or eating.") {
}

NoSlowDown::~NoSlowDown() {
}

const char* NoSlowDown::getModuleName() {
	return ("NoSlowDown");
}

void NoSlowDown::onEnable() {
	Utils::nopBytes((BYTE*)((uintptr_t)cancelSlowDown1), 5);
	Utils::nopBytes((BYTE*)((uintptr_t)cancelSlowDown2), 5);
	Utils::nopBytes((BYTE*)((uintptr_t)cancelSlowDown3), 5);
	Utils::nopBytes((BYTE*)((uintptr_t)cancelSlowDown4), 5);
}

void NoSlowDown::onDisable() {
	Utils::patchBytes((BYTE*)((uintptr_t)cancelSlowDown1), (BYTE*)("\xF3\x0F\x11\x46\x08"), 5);
	Utils::patchBytes((BYTE*)((uintptr_t)cancelSlowDown2), (BYTE*)("\xF3\x0F\x11\x46\x0C"), 5);
	Utils::patchBytes((BYTE*)((uintptr_t)cancelSlowDown3), (BYTE*)("\xF3\x0F\x11\x46\x08"), 5);
	Utils::patchBytes((BYTE*)((uintptr_t)cancelSlowDown4), (BYTE*)("\xF3\x0F\x11\x46\x0C"), 5);
}
