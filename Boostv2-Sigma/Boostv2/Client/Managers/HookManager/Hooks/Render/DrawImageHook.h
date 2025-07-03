#pragma once
#include "../FuncHook.h"
#include "../../Client/Client.h"
Vec2<float> hotbarPos = Vec2<float>(0, 0);

Vec2<float> Lerp(Vec2<float> a, Vec2<float> b, float t) {
	return Vec2<float>(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
}
class DrawImageHook : public FuncHook {
private:
	using func_t = __int64(__fastcall*)(MinecraftUIRenderContext*, TexturePtr*, Vec2<float>&, Vec2<float>&, Vec2<float>&, Vec2<float>&);
	static inline func_t oFunc;

	static __int64 DrawImageCallback(MinecraftUIRenderContext* ctx, TexturePtr* texture, Vec2<float>& pos, Vec2<float>& size, Vec2<float>& uvPos, Vec2<float>& uvSize) {
		if (!texture || !texture->texture)
			return oFunc(ctx, texture, pos, size, uvPos, uvSize);
		const char* texPath = texture->texture->filePath.empty() ? nullptr : texture->texture->filePath.c_str();
		if (texPath && strcmp(texPath, "textures/ui/title") == 0) {
			if (texPath) {
			    if (!MCR::renderCtx) return 0;
				if (texPath && strcmp(texPath, "textures/ui/title") == 0) {
					if (!MCR::renderCtx) return 0;

					static const std::string text = "Boost";
					static Colors* colorsMod = ModuleManager::getModule<Colors>();
					float scale = 7.5f;

					float textWidth = MCR::getTextWidth(text, scale);

					float textX = pos.x + (size.x / 2.f) - (textWidth / 2.f);
					float textY = pos.y;

					Vec2<float> textPos = { textX, textY };

					MCR::drawText(textPos, text, colorsMod->getColor().toMCColor(), scale);
					MCR::renderCtx->flushText(1.0f);

					return 0;
				}
			}
		}
		return oFunc(ctx, texture, pos, size, uvPos, uvSize);
	}

public:
	DrawImageHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&DrawImageCallback;
	}
};
