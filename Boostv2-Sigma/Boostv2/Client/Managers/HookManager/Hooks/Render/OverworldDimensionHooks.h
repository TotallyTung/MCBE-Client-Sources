#pragma once
#include "../FuncHook.h"
#include "../../../ModuleManager/Modules/Category/Render/CustomSky.h"

class BrightnessFogColorHook : public FuncHook {
private:
    using func_t = float* (__fastcall*)(float*, void*, void*, void*);
    static inline func_t oFunc;

    static float* HookCallback(float* a1, void* a2, void* a3, void* a4) {
        if (!ModuleManager::init)
            return oFunc(a1, a2, a3, a4);

		static CustomSky* CustomSkyMod = ModuleManager::getModule<CustomSky>();

		if (CustomSkyMod->isEnabled() && CustomSkyMod->night) {
			static float nightColor[4] = { 0.f, 0.f, 0.2f, 1.f };
			return nightColor;
		}

		static float finalColor[4];

		if (CustomSkyMod->isEnabled()) {
			if (CustomSkyMod->custom) {
				static float h = 0.0f, s = 0.8f, v = 1.0f;

				ColorUtil::ColorConvertHSVtoRGB(h, s, v, finalColor[0], finalColor[1], finalColor[2]);
				finalColor[3] = 1.0f;

				h += CustomSkyMod->intensity;
				if (h >= 1.0f)
					h = 0.0f;

				return finalColor;
			}
			else {
				finalColor[0] = CustomSkyMod->color.r;
				finalColor[1] = CustomSkyMod->color.g;
				finalColor[2] = CustomSkyMod->color.b;
				finalColor[3] = 1.0f;

				return finalColor;
			}
		}


        return oFunc(a1, a2, a3, a4);
    }

public:
    BrightnessFogColorHook() {
        OriginFunc = (void**)&oFunc;
        func = (void*)&HookCallback;
    }
};
