#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/LoopbackPacketSender.h"

#include "../../../Client.h"

class NineSlicehook {
protected:
	using func_t = void(__fastcall*)(MinecraftUIRenderContext*, TextureData*, void*);
	static inline func_t oFunc;

	static void LoopbackPacketSender_sendToServer(MinecraftUIRenderContext* _this,TextureData* texturePtr,void* nineSliceInfo) {
		if (strcmp(texturePtr->ptrToPath->filePath.getText(), "textures/ui/focus_border_white") == 0 ||
			strcmp(texturePtr->ptrToPath->filePath.getText(), "textures/ui/slider_border") == 0 || strcmp(texturePtr->ptrToPath->filePath.getText(), "textures/ui/loading_bar") == 0)
		{
			return;
		}
		oFunc(_this, texturePtr, nineSliceInfo);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("NineSlicehook", address, (void*)&NineSlicehook::LoopbackPacketSender_sendToServer, (void*)&oFunc);
	}
};