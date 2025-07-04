#include "NoSlow.h"

#include <Windows.h>

#include "../../../../Utils/Logger.h"
#include "../../../../Utils/Utils.h"

using namespace std;
NoSlow::NoSlow() : IModule(0, Category::MOVEMENT, "Prevents you from slowing down when using items") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	registerBoolSetting("NoWeb", &web, web);
	registerBoolSetting("AntiImmobile", &this->antii, this->antii);
	registerBoolSetting("Boost", &this->spoof, this->spoof);
}

const char* NoSlow::getRawModuleName() {
	return "NoSlow";
}

const char* NoSlow::getModuleName() {
	//if (mode.getSelectedValue() == 0) name = string("NoSlow ") + string(GRAY) + string("Vanilla");
	name = string("NoSlow ") + string(GRAY) + string("Vanilla");
	return name.c_str();
}
void NoSlow::onMove(C_MoveInputHandler* input)
{
	if (spoof)
		auto player = g_Data.getLocalPlayer();
	if (spoof)
		if ((GameData::isRightClickDown()) && GameData::canUseMoveKeys())
		{
			if (spoof)
				input->forwardMovement *= 10;
			input->sideMovement *= 10;
		}
}
void NoSlow::onEnable() {
	switch (mode.getSelectedValue()) {
	// Vanilla
	case 0:
		if (opcode == 0 || opcode1 == 0) {
			opcode = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 0C F3 0F 10 05"));
			opcode1 = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 0C 41 C7"));
		}

		DWORD oldProtect = 0;
		if (!VirtualProtect(opcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
		#endif
		}
		else {
			opcode[0] = 0x90;
			opcode[1] = 0x90;
			opcode[2] = 0x90;
			opcode[3] = 0x90;
			opcode[4] = 0x90;
			VirtualProtect(opcode, 5, oldProtect, &oldProtect);
		}

		if (!VirtualProtect(opcode1, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
#endif
		}
		else {
			opcode1[0] = 0x90;
			opcode1[1] = 0x90;
			opcode1[2] = 0x90;
			opcode1[3] = 0x90;
			opcode1[4] = 0x90;
			VirtualProtect(opcode1, 5, oldProtect, &oldProtect);
		}
		break;
	}
}

void NoSlow::onTick(C_GameMode* gm) {
	// Vanilla
	if (mode.getSelectedValue() == 0) gm->player->slowdownFactor = vec3_t{0,0,0};
	if (web)
		gm->player->slowdownFactor = { 0, 0, 0 };
}

void NoSlow::onDisable() {
	switch (mode.getSelectedValue()) {
		// Vanilla
	case 0:
		if (opcode == 0 || opcode1 == 0) {
			opcode = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 0C F3 0F 10 05"));
			opcode1 = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 0C 41 C7"));
		}

		DWORD oldProtect = 0;
		if (!VirtualProtect(opcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
		#endif
		}
		else {
			opcode[0] = 0xF3;
			opcode[1] = 0x0F;
			opcode[2] = 0x11;
			opcode[3] = 0x46;
			opcode[4] = 0x0C;
			//opcode[5] = {0xF3; 0x0F, 0x11, 0x46, 0x0C};
			VirtualProtect(opcode, 5, oldProtect, &oldProtect);
		};

		if (!VirtualProtect(opcode1, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
		#endif
		}
		else {
			opcode1[0] = 0xF3;
			opcode1[1] = 0x0F;
			opcode1[2] = 0x11;

			opcode1[3] = 0x46;

			opcode1[4] = 0x0C;
			//opcode[5] = {0xF3; 0x0F, 0x11, 0x46, 0x0C};
			VirtualProtect(opcode1, 5, oldProtect, &oldProtect);
		};
		break;
	}
}