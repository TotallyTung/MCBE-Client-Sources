#pragma once
#include "SlimMem.h"

namespace Game {
	class Core {
	public:
		static inline ClientInstance* clientInstance;
		static inline const SlimUtils::SlimModule* gameModule = 0;
		static inline SlimUtils::SlimMem slimMem;

		// ClickGui Stuff
		static inline int TimeMode = 2;
		static inline float DayTimeValue = 1;

		static inline float SwingSpeed = 17;
	};

	class Pointers {
	public:
		static inline void* ScreenModel = nullptr;
	};

	bool openURL(std::string url) {
		if (Pointers::ScreenModel == nullptr) return false;
		static uintptr_t addr = findSig(xorstr_("48 89 5c 24 ? 57 48 83 ec ? 48 8b da 48 8b f9 e8 ? ? ? ? 84 c0 0f 84"));
		TextHolder  str(url);
		CallFunc<void, void*, TextHolder*>(addr, (void*)Pointers::ScreenModel, &str);
		return true;
	}

	float initProgress = 0.f;
	bool Initializing = false;

	ClientInstance* GetInstance() { return Core::clientInstance; }

	GameMode* _gm = nullptr;
	Actor* actorplayer;
	void setGameMode(GameMode* gm) { _gm = gm; }
	GameMode* getGameMode() { return _gm; }
	Actor* getActor() {
		return actorplayer;
	}

	static std::string ClientName = "Forge Client";
	static std::map<int, bool> Keymap;
	static bool isRunning = true;
	static bool openClickGUI = false;
	long long startMs = getCurrentMs();
	namespace RenderInfo
	{
		static inline HWND window = nullptr;
		static inline Vector2<float> windowSize = Vector2<float>{};
		static inline std::string bootLogLOL;
		static inline ID3D11ShaderResourceView* kairunTexture = NULL;
	}
};