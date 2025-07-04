#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Libs/ImGui/imgui.h"

#include "../../../Client.h"

class KeyMouseHook {
protected:
	using func_t = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
	static inline func_t func;

	static void clickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
		if (ImGui::GetCurrentContext() != nullptr) {
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
		static ClickGui* clickGuiMod = (ClickGui*)client->moduleMgr->getModule("ClickGui");
		if (clickGuiMod->isEnabled()) {
			clickGuiMod->onMouseUpdate(mouseButton, isDown);
			isDown = false;
		}
		func(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::KeyMouseHook);
		MemoryUtils::CreateHook("KeyMouseHook", address, (void*)&KeyMouseHook::clickFunc, (void*)&func);
	}
};