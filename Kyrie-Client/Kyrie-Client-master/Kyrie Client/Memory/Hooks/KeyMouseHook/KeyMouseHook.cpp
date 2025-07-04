#include "KeyMouseHook.h"
#include "../../Memory.h"
#include "../../../ImGui/imgui.h"
#include "../../../Client/Client.h"
#include "../../GameData/GameData.h"

typedef __int64(__fastcall* Mouse)(__int64, char, char, __int16, __int16, __int16, __int16, char);
Mouse _Mouse;

__int64 KeyMouseHook::KeyMouseCallBack(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	if (client->isInitialized() && ImGui::GetCurrentContext() != nullptr) {
		ImGuiIO& io = ImGui::GetIO();
		if (mouseButton > 0 && mouseButton < 3) {
			io.AddMouseButtonEvent(mouseButton - 1, isDown);
		}
		else if (mouseButton == 4) {
			io.AddMouseWheelEvent(0, (isDown < 0) ? -0.5f : 0.5f);
		}
		else {
			io.AddMousePosEvent(mouseX, mouseY);
		}
		if (io.WantCaptureMouse && isDown) {
			isDown = false;
		}
	}

	if (mouseButton == 1) {
		mc.isLeftClickDown = isDown;
		mc.isHoldingLeftClick = isDown;
	}
	else if (mouseButton == 2) {
		mc.isRightClickDown = isDown;
		mc.isHoldingRightClick = isDown;
	}

	static ClickGui* clickGuiMod = client->moduleMgr->getModule<ClickGui>();
	if (clickGuiMod->isEnabled()) isDown = false;

	return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

void KeyMouseHook::init() {
	uintptr_t address = Memory::getBase() + 0x3F1F420;  // 48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 54 41 55 41 56 41 57 48 83 EC 60 44 0F B7 BC 24

	Memory::CreateHook("KeyMouseHook", address, &KeyMouseHook::KeyMouseCallBack, &_Mouse);
}