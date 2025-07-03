#pragma once
#include "../FuncHook.h"

class KeyMouseHook : public FuncHook {
private:
	using func_t = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
	static inline func_t oFunc;

	static void mouseInputCallback(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {

		Game::keyMousePtr = (void*)(a1 + 0x10);

		static ClickGUI* clickGuiMod = ModuleManager::getModule<ClickGUI>();
		if (clickGuiMod->isEnabled()) {
			clickGuiMod->onMouseUpdate(Vec2<float>((float)mouseX, (float)mouseY), mouseButton, isDown);
			return;
		}
		oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	}
public:
	KeyMouseHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&mouseInputCallback;
	}
};