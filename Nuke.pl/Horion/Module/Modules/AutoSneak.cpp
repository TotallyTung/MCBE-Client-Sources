#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0, Category::MOVEMENT, "Automatically AutoSneaks") {
	registerBoolSetting("Silent", &silentlol, silentlol);
}

const char* AutoSneak::getModuleName() {
	return "AutoSneak";
}

const char* AutoSneak::getModName() {
	if (!silentlol) {
		name = std::string("Normal");
	} else {
		name = std::string("Silent");
	}
	return name.c_str();
}

void AutoSneak::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
}

void AutoSneak::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}
uint8_t* AutoSneakopcode = 0;
void AutoSneak::onPlayerTick(Player* plr) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (silentlol) {
		if (AutoSneakopcode == 0) {
			AutoSneakopcode = reinterpret_cast<uint8_t*>(FindSignature("80 7B 48 00 74 2E 48 8B 07 48 8B CF"));
		}
		DWORD oldProtect = 0;
		if (!VirtualProtect(AutoSneakopcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		} else {
			AutoSneakopcode[0] = 0x90;
			AutoSneakopcode[1] = 0x90;
			AutoSneakopcode[2] = 0x90;
			AutoSneakopcode[3] = 0x90;
			VirtualProtect(AutoSneakopcode, 5, oldProtect, &oldProtect);
		}
	}
}
void AutoSneak::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}

void AutoSneak::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	DWORD oldProtect = 0;
	if (VirtualProtect(AutoSneakopcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		AutoSneakopcode[0] = 0x80;
		AutoSneakopcode[1] = 0x7B;
		AutoSneakopcode[2] = 0x48;
		AutoSneakopcode[3] = 0x00;
		VirtualProtect(AutoSneakopcode, 5, oldProtect, &oldProtect);
	}
}