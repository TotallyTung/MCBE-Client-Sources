#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/ColorUtils.h"
#include "../../../../SDK/Classes/ScreenContext.h"
#include "../../../../SDK/Classes/TextHolder.h"

class FontDrawTransformedHook {
protected:
	using func_t = __int64(__thiscall*)(void*, ScreenContext*, TextHolder*, float, float, MC_Color, float, float, bool, float);
	static inline func_t func;

	static void FontDrawTransformedCallback(void* _this, ScreenContext* screenContext, TextHolder* textStr, float posX, float posY, MC_Color color, float angle, float scale, bool centered, float maxWidth) {
		static TextHolder text("riel poopbob clien");
		//static int mmb = 0;
		//mmb++;
		//if (mmb >= 360) mmb = 0;
		//angle = mmb; // Spin lfmao

		static float dScale = 1.5f; // dynamic Scale
		static bool up = true;
		if (up) {
			dScale += 0.01f;
			if (dScale >= 1.6f) up = false;
		}
		else {
			dScale -= 0.01f;
			if (dScale <= 1.4f) up = true;
		}
		scale = dScale;
		func(_this, screenContext, &text, posX, posY, color, angle, scale, centered, maxWidth);
	}

public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::FontDrawTransformedHook);
		MemoryUtils::CreateHook("FontDrawTransformedHook", address, (void*)&FontDrawTransformedHook::FontDrawTransformedCallback, (void*)&func);
	}
};