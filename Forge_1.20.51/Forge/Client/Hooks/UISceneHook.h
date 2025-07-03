#pragma once
#include <inttypes.h>
void* oUIScene;

void UISceneRender(
	void* uiscene,
	void* screenctx
)
{
	__int64 screenview = *(__int64*)((__int64)uiscene + 0x38);
	__int64 screencontroller = *(__int64*)(screenview + 0x38);
	__int64 screenmodel = *(__int64*)(screencontroller + 0x928);
	//printf("UIScene | %" PRIx64 " | %" PRIx64 "\n", uiscene, screenmodel);
	Game::Pointers::ScreenModel = (void*)screenmodel;
	CallFunc<void*, void*, void*>(
		oUIScene,
		uiscene,
		screenctx
	);
}

class UISceneHook : public FuncHook {
public:

	static UISceneHook& Get() {
		static UISceneHook instance;
		return instance;
	}

	bool Initialize() override {
		uintptr_t addr = findSig(xorstr_("48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B FA 48 8B D9 B9 ? ? ? ?"), "UIScene::Render");
		if (not HookFunction((void*)addr, (void*)&UISceneRender, &oUIScene, xorstr_("UIScene")))
			return false;
		return true;
	}
};
