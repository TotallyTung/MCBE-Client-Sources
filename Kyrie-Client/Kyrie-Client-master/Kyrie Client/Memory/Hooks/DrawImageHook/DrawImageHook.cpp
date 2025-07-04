#include "DrawImageHook.h"
#include "../../Memory.h"

typedef __int64(__fastcall* drawImage)(MinecraftUIRenderContext*, TexturePtr*, float*, float*, __int64, __int64);
drawImage _drawImage;

__int64 DrawImageHook::DrawImageCallBack(MinecraftUIRenderContext* renderCtx, TexturePtr* texturePtr, float* a3, float* a4, __int64 a5, __int64 a6) {
	if (strcmp(texturePtr->filePath->filePathStr.c_str(), "textures/ui/selected_hotbar_slot") == 0) {
		static float PosX = a3[0];
		PosX = lerpSync(PosX, a3[0], 0.15f);
		a3[0] = PosX;
	}
	return _drawImage(renderCtx, texturePtr, a3, a4, a5, a6);
}

void DrawImageHook::init() {
	uintptr_t address = Memory::getBase() + 0x131BEB0; // 48 8B C4 48 89 58 10 48 89 68 18 48 89 70 20 57 41 54 41 55 41 56 41 57 48 81 EC 90 00 00 00 0F 29 70 C8 0F

	Memory::CreateHook("DrawImageHook", address, &DrawImageHook::DrawImageCallBack, &_drawImage);
}