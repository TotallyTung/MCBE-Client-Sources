#pragma once
#include "ClientInstance.h"
#include "ScreenContext.h"
#include "FontBitmap.h"
#include "TextHolder.h"
#include "TextMeasureData.h"
#include "CaretMeasureData.h"
#include "TextureData.h"

#include "../../Utils/ColorUtils.h"
class MinecraftUIRenderContext {
public:
	ClientInstance* clientInstance; // 0x8
	ScreenContext* screenContext; // 0x10
public:
	virtual ~MinecraftUIRenderContext();
	virtual float getLineLength(FontBitmap* font, TextHolder* str, float textSize, bool isCalcWidth);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual __int64 drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(FontBitmap* font, const float* pos, std::string* text, MC_Color* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
	virtual __int64 drawImage(const TextureData* texturePtr, const Vec2<float>& imagePos, const Vec2<float>& imageDimension, const Vec2<float>& uvPos, const Vec2<float>& uvSize);
	virtual __int64 drawNineslice(TextureData* texturePtr, __int64* ninesliceInfo);
	virtual __int64 flushImages(MC_Color& color, float timeSinceLastFlush, class HashedString& hashedString);
	virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void endSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	virtual void increaseStencilRef(void);
	virtual void decreaseStencilRef(void);
	virtual void resetStencilRef(void);
	virtual void fillRectangleStencil(float*);
	TextureData* getTexture(TextureData* texture, ResourceLocation* resourceLocation)
	{
		return MemoryUtils::CallVFunc<29, TextureData*, TextureData*, ResourceLocation*>(this, texture, resourceLocation);
	};


};