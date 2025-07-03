#pragma once
#include "../Utils/HMath.h"
#include "TextHolder.h"
#include <vector>

struct MC_Color;
class FilePath {
private:
	uint64_t type;
	TextHolder filePath;  //0x0008
public:
	FilePath(std::string filePath, bool system = false) {
		memset(this, 0, sizeof(FilePath));
		this->filePath.setText(filePath);
		if (system)
			this->type = 2;
	};
};
class FilePath2 {
private:
	char pad_0x0[0x18];    //0x0000
public:
	TextHolder filePathStr;  //0x0018
};
class TexturePtr {
private:
	char padding[24];
	struct FilePath {
		char padding[24];
		TextHolder filePath;
	}*path;
	char padding2[32];
public:
	TextHolder getFilePath() const {
		return path->filePath;
	}
};

class TexturePtr2 {
private:
	char pad_0x0[0x18];  //0x0000
public:
	FilePath2* filePath2;  //0x0018
private:
	char pad_0x0038[0x20];  //0x0038
};

class Font {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();

public:
	virtual float getLineLength(TextHolder* str, float textSize);  //bool unknown
	virtual float getLineHeight();
};

struct TextMeasureData {
	float textSize;
	int idk;
	bool displayShadow;
	bool bool2;
};
class MinecraftUIRenderContext2 {
private:
	virtual auto Constructor() -> void {};

public:
	virtual float getLineLength(Font* font, TextHolder* str, float textSize);  //bool unknown
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(Font* font, const float* pos, TextHolder* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
	virtual __int64 drawImage(const TexturePtr* a2, Vec2 const& ImagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize);
	virtual void drawNineslice(TexturePtr* texturePtr, void* nineslice);
	virtual __int64 flushImages(MC_Color& color, float alpha, HashedString hashedString);
	virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void endSharedMeshBatch(float timeSinceLastFlush);
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);  // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	virtual void increaseStencilRef();
	virtual void decreaseStencilRef();
	virtual void resetStencilRef();
	virtual void fillRectangleStencil(Vec4 position);
	virtual void enableScissorTest(Vec4 position);
	virtual void disableScissorTest();
	virtual void setClippingRectangle(Vec4 position);
	virtual void setFullClippingRectangle();
	virtual void saveCurrentClippingRectangle();
	virtual void restoreSavedClippingRectangle();
	virtual int getFullClippingRectangle();
	virtual void updateCustom(uintptr_t a1);
	virtual void renderCustom(uintptr_t a1, int a2, Vec4 position);
	virtual void cleanup();
	virtual void removePersistentMeshes();
	virtual TexturePtr* getTexture(TexturePtr* texture, FilePath* resourceLocation, bool iDontFuckingKnowBruhPleaseHelpMeNigga);
	virtual int getZippedTexture(TexturePtr* Path, TexturePtr* ResourceLocation, bool a3);
};

class ResourceLocation;
class MinecraftUIRenderContext {
public:
	class ClientInstance* clientInstance;  // 0x0008
	class ScreenContext* screenContext;    // 0x0010

	float getLineLength(Font* font, TextHolder* str, float textSize, bool unknown) {
		return Utils::CallVFunc<1, float, Font*, TextHolder*, float, bool>(this, font, str, textSize, unknown);
	};
	float getTextAlpha() {
		return Utils::CallVFunc<2, float>(this);
	};
	void setTextAlpha(float alpha) {
		return Utils::CallVFunc<3, void, float>(this, alpha);
	};
	__int64 drawDebugText(const float* pos, TextHolder* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData) {
		return Utils::CallVFunc<4, __int64, const float*, TextHolder*, const float*, float, unsigned int, const TextMeasureData*, const uintptr_t*>(this, pos, text, color, alpha, textAlignment, textMeasureData, caretMeasureData);
	};
	__int64 drawText(Font* font, const float* pos, TextHolder* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData) {
		return Utils::CallVFunc<5, __int64, Font*, const float*, TextHolder*, const float*, float, unsigned int, const TextMeasureData*, const uintptr_t*>(this, font, pos, text, color, alpha, textAlignment, textMeasureData, caretMeasureData);
	};
	void flushText(float timeSinceLastFlush) {
		return Utils::CallVFunc<6, void, float>(this, timeSinceLastFlush);
	};
	__int64 drawImage(const TexturePtr* a2, Vec2 const& ImagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize) {
		return Utils::CallVFunc<7, __int64, const TexturePtr*, Vec2 const&, Vec2 const&, Vec2 const&, Vec2 const&>(this, a2, ImagePos, ImageDimension, uvPos, uvSize);
	};
	__int64 drawNineslice(TexturePtr* texturePtr, __int64* ninesliceInfo) {
		return Utils::CallVFunc<8, __int64, TexturePtr*, __int64*>(this, texturePtr, ninesliceInfo);
	};
	__int64 flushImages(MC_Color& color, float opacity, class HashedString& hashedString) {
		return Utils::CallVFunc<9, __int64, MC_Color&, float, class HashedString&>(this, color, opacity, hashedString);
	};
	void drawRectangle(Vec4 const& pos, MC_Color& color, float alpha, int lineWidth) {
		return Utils::CallVFunc<12, void, Vec4 const&, MC_Color&, float, int>(this, pos, color, alpha, lineWidth);
	};
	void fillRectangle(Vec4 const& pos, MC_Color& color, float alpha) {
		return Utils::CallVFunc<13, void, Vec4 const&, MC_Color&, float>(this, pos, color, alpha);
	};
	void setClippingRectangle(Vec4 const& pos) {
		return Utils::CallVFunc<20, void, Vec4 const&>(this, pos);
	};
	void saveCurrentClippingRectangle() {
		return Utils::CallVFunc<22, void>(this);
	};
	void restoreSavedClippingRectangle() {
		return Utils::CallVFunc<23, void>(this);
	};
	TexturePtr* getTexture(TexturePtr* texture, ResourceLocation* resourceLocation, bool iDontFuckingKnowBruhPleaseHelpMeNigga) {
		return Utils::CallVFunc<29, TexturePtr*, TexturePtr*, ResourceLocation*, bool>(this, texture, resourceLocation, iDontFuckingKnowBruhPleaseHelpMeNigga);
	};
	virtual TexturePtr* getTexture2(TexturePtr* texture, FilePath* resourceLocation, bool iDontFuckingKnowBruhPleaseHelpMeNigga);
};