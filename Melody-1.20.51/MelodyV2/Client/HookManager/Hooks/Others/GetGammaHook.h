// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Options.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class GetGammaHook {
protected:
	using func_t = float(__thiscall*)(Options**);
	static inline func_t func;

	static float GetGamma(Options** optionList) {
		static NoRender* noRenderMod = (NoRender*)client->moduleMgr->getModule("NoRender");
		static Fullbright* fullBrightMod = (Fullbright*)client->moduleMgr->getModule("Fullbright");

		uintptr_t** list = (uintptr_t**)optionList;
		int obtainedSettings = 0;
		for (uint16_t i = 3; i < 450; i++) {
			if (list[i] == nullptr) continue;
			uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 8);
			if (info == nullptr) continue;

			TextHolder* translateName = (TextHolder*)((uintptr_t)info + 0x158);
			bool* boolSetting = (bool*)((uintptr_t)list[i] + 16);
			if (strcmp(translateName->getText(), "options.dev_disableRenderWeather") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->noWeather;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderEntities") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->noEntities;
				obtainedSettings++;
			}
			//new
			else if (strcmp(translateName->getText(), "options.dev_disableRenderSky") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->noSky;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderHud") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->NoHUD;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderItemInHand") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->Noiteminhand;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderTerrain") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->NoTerrain;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderMainMenuPaperdollAnimation") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->NoMainMenuPaperdollAnimation;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderMainMenuCubeMap") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->NoMainMenuCubeMap;
				obtainedSettings++;
			}
			//new
			else if (strcmp(translateName->getText(), "options.dev_disableRenderBlockEntities") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->noBlockEntities;
				obtainedSettings++;
			}
			else if (strcmp(translateName->getText(), "options.dev_disableRenderParticles") == 0) {
				*boolSetting = noRenderMod->isEnabled() && noRenderMod->noParticles;
				obtainedSettings++;
			}

			if (obtainedSettings == 4) break;

		}
		if (fullBrightMod->isEnabled()) {
			return fullBrightMod->intensity;
		}
		return func(optionList);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::GetGammaHook);
		MemoryUtils::CreateHook("GetGammaHook", address, (void*)&GetGammaHook::GetGamma, (void*)&func);
	}
};
