// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Options.h"
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/MemoryUtils.h"
#include "../../../Client.h"

class GetGlintColorHook {
protected:
	using func_t = float* (__thiscall*)(float* , __int64 , int , float* );
	static inline func_t func;

	static float* GetGlintColor(float* color, __int64 a2, int a3, float* a4) {
		static GlintColor* CustomSkyMod = (GlintColor*)client->moduleMgr->getModule("GlintColor");
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
		return func(color, a2, a3, a4);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::glintscolorhook);
		MemoryUtils::CreateHook("GetGlintColorHook", address, (void*)&GetGlintColorHook::GetGlintColor, (void*)&func);
	}
};
