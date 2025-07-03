#pragma once
void* oNetSkip;

class RakPeerHooks : public FuncHook {
public:
	static __int64 RakPeer_RunUpdateCycle(__int64 _this, __int64 a1) {
		if (Game::Keymap[90]) { // The Bind 'Z' cuz its cool >:)
			return 0;
		}
		return CallFunc<__int64, __int64, __int64>(oNetSkip, _this, a1);
	}

	static RakPeerHooks& Get() {
		static RakPeerHooks instance;
		return instance;
	}

	bool Initialize() override {
		uintptr_t addr = findSig(xorstr_("48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? b8 ? ? ? ? e8 ? ? ? ? 48 2b e0 0f 29 b4 24 ? ? ? ? 0f 29 bc 24 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 8b fa 48 89 55 ? 4c 8b e1"));
		if (not HookFunction((void*)addr, (void*)&RakPeer_RunUpdateCycle, &oNetSkip, xorstr_("RakPeer_RunUpdateCycle")))
			return false;
		return true;
	}
};
