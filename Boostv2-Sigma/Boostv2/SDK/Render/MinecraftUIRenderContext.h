#pragma once
#include "../Core/ClientInstance.h"
#include "ScreenContext.h"

#include "RectangleArea.h"
#include "Font.h"
#include "TextMeasureData.h"
#include "CaretMeasureData.h"
#include "../../Libs/imgui/imgui.h"

class ResourceLocation {
public:
	uint64_t type;        //0x0000
	std::string filePath;  //0x0008

	ResourceLocation(std::string filePath, bool external) {
		memset(this, 0, sizeof(ResourceLocation));
		this->filePath = std::move(filePath);
		if (external)
			this->type = 2;
	};
};

class TexturePtr {
private:
	char padding[0x10];
public:
	std::shared_ptr<ResourceLocation> texture;
};

class MinecraftUIRenderContext {
public:
	ClientInstance* clientInstance;
	ScreenContext* screenContext;
public:
	virtual ~MinecraftUIRenderContext();
	virtual float getLineLength(Font* font, std::string* str, float textSize, bool showColorSymbol);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText(RectangleArea* rect, std::string* text, mce::Color* color, float alpha, unsigned int textAlignment, TextMeasureData* textMeasureData, CaretMeasureData* caretMeasureData);
	virtual void drawText(Font* font, RectangleArea* rect, std::string* text, mce::Color* color, float alpha, unsigned int textAlignment, TextMeasureData* textMeasureData, CaretMeasureData* caretMeasureData);
	virtual void flushText(float deltaTime);
};
