#include "GhostHand.h"

GhostHand::GhostHand() : IModule(0, Category::PLAYER, "Hit people through blocks, this may cause config to crash on load if saved while enabled") {
}

GhostHand::~GhostHand() {
}

const char* GhostHand::getRawModuleName() {
	return "GhostHand";
}
void* target;

void GhostHand::onEnable() {
	target = (void*)(FindSignature("76 0F C6 44 24 50 01"));

	Utils::nopBytes((unsigned char*)((uintptr_t)target), 7);
}

void GhostHand::onDisable() {
	unsigned char* patch = (unsigned char*)"\x76\x0F\xC6\x44\x24\x50\x01";
	Utils::patchBytes((unsigned char*)((uintptr_t)target), patch, 7);
}
