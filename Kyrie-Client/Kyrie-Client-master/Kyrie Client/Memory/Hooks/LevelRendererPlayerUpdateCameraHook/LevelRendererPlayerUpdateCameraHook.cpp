#include "LevelRendererPlayerUpdateCameraHook.h"
#include "../../Memory.h"
#include "../../../ImGui/imgui.h"
#include "../../../Client/Client.h"
#include "../../GameData/GameData.h"

typedef __int64(__fastcall* updateCamera)(LevelRendererPlayer*, __int64);
updateCamera _updateCamera;

__int64 LevelRendererPlayerUpdateCameraHook::updateCamera(LevelRendererPlayer* a1, __int64 a2) {
	return _updateCamera(a1, a2);

	//return 0;
	//__int64 result = _updateCamera(a1, a2);
	//return result;
}

void LevelRendererPlayerUpdateCameraHook::init() {
	uintptr_t address = Memory::getBase() + 0x13B6260;  // 40 55 56 57 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48

	Memory::CreateHook("LevelRendererPlayerUpdateCameraHook", address, &LevelRendererPlayerUpdateCameraHook::updateCamera, &_updateCamera);
}