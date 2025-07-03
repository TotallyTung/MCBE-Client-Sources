#pragma once
#include "../FuncHook.h"

class GetGammaHook : public FuncHook {
private:
	using func_t = float(__thiscall*)(__int64);
	static inline func_t oFunc;

	static float GetGammaCallback(__int64 a1) {

		static bool initPtr = false;
		if (!initPtr) {
			uintptr_t** list = (uintptr_t**)a1;
			ChunkBorders* chunkBordersMod = ModuleManager::getModule<ChunkBorders>();
			NoRender* noRenderMod = ModuleManager::getModule<NoRender>();

			int obtainedSettings = 0;
			for (uint16_t i = 3; i < 450; i++) {
				if (list[i] == nullptr) continue;
				uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 0x8);
				if (info == nullptr) continue;

				std::string* translateName = (std::string*)((uintptr_t)info + 0x158);
				bool* boolSetting = (bool*)((uintptr_t)list[i] + 16);

				if (strcmp(translateName->c_str(), "options.dev_showChunkMap") == 0) {
					chunkBordersMod->showChunkBorderBoolPtr = boolSetting;
					obtainedSettings++;
				}
				else if (strcmp(translateName->c_str(), "options.dev_disableRenderWeather") == 0) {
					noRenderMod->noWeatherBoolPtr = boolSetting;
					obtainedSettings++;
				}
				else if (strcmp(translateName->c_str(), "options.dev_disableRenderEntities") == 0) {
					noRenderMod->noEntitiesBoolPtr = boolSetting;
					obtainedSettings++;
				}
				else if (strcmp(translateName->c_str(), "options.dev_disableRenderBlockEntities") == 0) {
					noRenderMod->noBlockEntitiesBoolPtr = boolSetting;
					obtainedSettings++;
				}
				else if (strcmp(translateName->c_str(), "options.dev_disableRenderParticles") == 0) {
					noRenderMod->noParticlesBoolPtr = boolSetting;
					obtainedSettings++;
				}

				if (obtainedSettings == 5)
					break;
			}
			initPtr = true;
		}

		static Fullbright* fullBrightMod = ModuleManager::getModule<Fullbright>();
		if (fullBrightMod->isEnabled())
			return 12.f;

		return oFunc(a1);
	}
public:
	GetGammaHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetGammaCallback;
	}
};