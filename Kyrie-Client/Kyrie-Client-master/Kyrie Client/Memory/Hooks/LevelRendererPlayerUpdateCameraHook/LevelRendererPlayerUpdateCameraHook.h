#pragma once
#include "../../../SDK/LevelRendererPlayer.h"
class LevelRendererPlayerUpdateCameraHook {
private:
	static __int64 updateCamera(LevelRendererPlayer* a1, __int64 a2);
public:
	static void init();
};
