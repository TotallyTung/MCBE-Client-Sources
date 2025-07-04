#pragma once
#include "../../../../SDK/Classes/Options.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class GetFogColorHook {
protected:
	using func_t = float*(__thiscall*)(Dimension*, float*, __int64, float);
	static inline func_t func;

	static float* GetFogColor(Dimension* _this, float* color, __int64 a3, float a4) {
		static CustomSky* CustomSkyMod = (CustomSky*)client->moduleMgr->getModule("CustomSky");
		MC_Color rColor = ColorUtils::getRainbowColor(3, 0.5f, 1, 1).toMC_Color();
		
		if (CustomSkyMod->isEnabled() && CustomSkyMod->night) {
			color[0] = 0.f;
			color[1] = 0.f;
			color[2] = 0.2f;
			color[3] = 1;
			return color;
		}

		
		static float rcolors[4];
		static float currColor[4];

		if (CustomSkyMod->isEnabled()) {
			if (CustomSkyMod->custom) {
				if (rcolors[3] < 1) {
					rcolors[0] = 1;
					rcolors[1] = 0.2f;
					rcolors[2] = 0.2f;
					rcolors[3] = 1;
				}

				ColorUtils::RGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, don't question this

				rcolors[0] += CustomSkyMod->intensity;
				if (rcolors[0] >= 1) {
					rcolors[0] = 0;
				}

				ColorUtils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

				return rcolors;
			}
			else {
				if (currColor[3] < 1) {
					currColor[0] = 1;
					currColor[1] = 0.2f;
					currColor[2] = 0.2f;
					currColor[3] = 1;
				}

				ColorUtils::RGBtoHSV(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);  // perfect code, don't question this

				currColor[0] += CustomSkyMod->intensity;
				if (currColor[0] >= 1) {
					currColor[0] = 0;
				}

				ColorUtils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

				currColor[0] = CustomSkyMod->color.r;
				currColor[1] = CustomSkyMod->color.g;
				currColor[2] = CustomSkyMod->color.b;

				return currColor;
			}
		}
		return func(_this, color, a3, a4);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::GetFogColorHook);
		MemoryUtils::CreateHook("GetFogColorHook", address, (void*)&GetFogColorHook::GetFogColor, (void*)&func);
	}
};
