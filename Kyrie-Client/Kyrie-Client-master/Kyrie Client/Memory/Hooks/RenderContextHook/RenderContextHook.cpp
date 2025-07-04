#include "RenderContextHook.h"
#include "../../Memory.h"
#include "../../GameData/GameData.h"

typedef __int64(__fastcall* renderContext)(__int64, MinecraftUIRenderContext*);
renderContext _renderContext;

__int64 RenderContextHook::RenderContextCallBack(__int64 a1, MinecraftUIRenderContext* renderCtx) {
	if (mc.getClientInstance() == nullptr) {
		mc.setClientInstance(renderCtx->clientInstance);
		//Utils::DebugF("%llx", (*((uintptr_t**)renderCtx))[7]);
	}
	return _renderContext(a1, renderCtx);
}

void RenderContextHook::init() {
	uintptr_t address = Memory::getBase() + 0x602EF0;  // 48 8B ? 48 89 ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? ? 4C 8B

	Memory::CreateHook("RenderContextHook", address, &RenderContextHook::RenderContextCallBack, &_renderContext);
}