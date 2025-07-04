#include "Sneak.h"
#include "../pch.h"

Sneak::Sneak() : IModule(0, Category::MOVEMENT, "Automatically sneaks") {
	registerBoolSetting("Silent", &silentlol, silentlol);
}

const char* Sneak::getRawModuleName() {
	return "AutoSneak";
}

const char* Sneak::getModuleName() {
	if (!silentlol) {
		name = std::string("AutoSneak ") + std::string(GRAY) + std::string("[") + std::string(WHITE) + std::string("Normal") + std::string(GRAY) + std::string("]");
	}
	else
	{
		name = std::string("AutoSneak ") + std::string(GRAY) + std::string("[") + std::string(WHITE) + std::string("Silent") + std::string(GRAY) + std::string("]");
	}
	return name.c_str();
}

void Sneak::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
}

void Sneak::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

}
uint8_t* sneakopcode = 0;
void Sneak::onPlayerTick(C_Player* plr) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (silentlol) {
		if (sneakopcode == 0) {
			sneakopcode = reinterpret_cast<uint8_t*>(FindSignature("80 7B 48 00 74 2E 48 8B 07 48 8B CF"));
		}
		DWORD oldProtect = 0;
		if (!VirtualProtect(sneakopcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		}
		else {
			sneakopcode[0] = 0x90;
			sneakopcode[1] = 0x90;
			sneakopcode[2] = 0x90;
			sneakopcode[3] = 0x90;
			VirtualProtect(sneakopcode, 5, oldProtect, &oldProtect);
		}
	}
}
void Sneak::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void Sneak::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	DWORD oldProtect = 0;
	if (VirtualProtect(sneakopcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		sneakopcode[0] = 0x80;
		sneakopcode[1] = 0x7B;
		sneakopcode[2] = 0x48;
		sneakopcode[3] = 0x00;
		VirtualProtect(sneakopcode, 5, oldProtect, &oldProtect);
	}
}