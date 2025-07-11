#pragma once
#include "../FuncHook.h"
#include "../DirectX/PresentHook.h"
#include "../../../../../Renderer/MCR.h"
#include "../../../../../Utils/TimerUtil.h"
#include "../../HookManager.h"
#include "DrawImageHook.h"

class SetUpAndRenderHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(ScreenView*, MinecraftUIRenderContext*);
	static inline func_t oFunc;

	static void ScreenView_SetUpAndRenderCallback(ScreenView* _this, MinecraftUIRenderContext* renderCtx) {
		// Hook drawImage once using renderCtx vtable
		static bool initialized = false;
		if (!initialized && renderCtx) {
			static DrawImageHook drawImageHook;
			void** ctxVtable = *(void***)renderCtx;
			drawImageHook.HookVirtual(ctxVtable, 7, "DrawImage");
			initialized = true;
		}

		// Call original render function
		oFunc(_this, renderCtx);

		// Debug screen render
		uintptr_t* visualTree = *(uintptr_t**)((uintptr_t)(_this)+0x48);
		std::string rootControlName = *(std::string*)((uintptr_t)(visualTree)+0x28);

		static std::string debugScreen = "debug_screen.debug_screen";
		if (strcmp(rootControlName.c_str(), debugScreen.c_str()) == 0) {
			MCR::onRenderScreen(renderCtx);
			MCR::deltaTime = _this->deltaTime;
		}
	}

public:
	SetUpAndRenderHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ScreenView_SetUpAndRenderCallback;
	}
};
