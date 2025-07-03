#pragma once
#include <inttypes.h>
void* oOpenURL;

class OpenURLHook : public FuncHook {
public:
	static void LaunchURL(void* a1, TextHolder* url) {
		printf("OPENURL | 0x%" PRIx64 " | % s\n", a1, url->getText());
		TextHolder  str("https://kairun.jp");
		return CallFunc<void, void*, TextHolder*>(oOpenURL, a1, &str);
	}

	static OpenURLHook& Get() {
		static OpenURLHook instance;
		return instance;
	}

	bool Initialize() override {
		uintptr_t addr = findSig(xorstr_("48 89 5c 24 ? 57 48 83 ec ? 48 8b da 48 8b f9 e8 ? ? ? ? 84 c0 0f 84"), "OpenURL");
		if (not HookFunction((void*)addr, (void*)&LaunchURL, &oOpenURL, xorstr_("OpenURL")))
			return false;
		return true;
	}
};
