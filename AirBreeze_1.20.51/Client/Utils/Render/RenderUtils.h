#pragma once
#include "../Utils.h"
#include "../../Include/imgui/imgui.h"
#include "../../SDK/Classes/MinecraftUIRenderContext.h"
#include "../../SDK/Minecraft.h"

class RenderUtils {
public:
	static class MinecraftUIRenderContext* context;

	/* Default colors */
	static Color white;
	static Color black;
	static Color red;
	static Color lime;
	static Color blue;
	static Color yellow;
	static Color cyan;
	static Color purple;

	static __forceinline void flush() {
		context->flushText(0);
	}

	static void drawImage(std::string path, Vec2<float>& imagePos, Vec2<float>& ImageDimension, Vec2<float>& uvPos, Vec2<float>& uvSize, ResourceFileSystem type = ResourceFileSystem::Raw)
{
	if (context == nullptr) return;

	ResourceLocation location = ResourceLocation(type, path);

	TexturePtr* texturePtr = new TexturePtr();
	context->getTexture(texturePtr, &location);

	if (texturePtr != nullptr)
	context->drawImage(texturePtr, &imagePos, &ImageDimension, &uvPos, &uvSize);
}

static void flushImage(Color color = Color(255.f, 255.f, 255.f, 1.f), float opacity = 1.f)
{
	if (context == nullptr) return;

	static HashedString flushString("ui_flush", 0xA99285D21E94FC80);
	context->flushImages(color, opacity, flushString);
}

	static __forceinline float getTextWidth(std::string text, float textSize, class Font* font) {
		return context->getLineLength(font, text, textSize, false);
	}

	static __forceinline float getTextHeight(class Font* font) {
		return font->getLineHeight();
	}

	static __forceinline void drawText(Font* font, Vec2<float> textPos, std::string text, Color color, float fontSize) {
		if (font == nullptr) {

			auto instance = context->clientInstance;
			auto mcgame = (instance != nullptr ? instance->MinecraftGame : nullptr);
			font = (mcgame != nullptr ? mcgame->mcfont : nullptr);

		};

		if (font == nullptr)
			return;

		TextMeasureData textMeasureData = TextMeasureData(fontSize);
		CaretMeasureData caretMeasureData = CaretMeasureData();

		auto textRect = Rect(textPos.x, textPos.x + (textPos.x * fontSize), textPos.y, textPos.y + (textPos.y * fontSize / 2));
		context->drawText(font, textRect.get(), text, color.get(), color.a, 0, &textMeasureData, &caretMeasureData);
	}

	static __forceinline float getTextLen(Font* font, std::string text, float fontSize) {
		if (font == nullptr) {

			auto instance = context->clientInstance;
			auto mcgame = (instance != nullptr ? instance->MinecraftGame : nullptr);
			font = (mcgame != nullptr ? mcgame->mcfont : nullptr);

		};

		if (font == nullptr)
			return 0.f;

		return context->getLineLength(font, text, fontSize, false);
	}

	static __forceinline void drawRectangle(Rect rect, Color color, int lineWidth) {
		rect = Rect(rect.x, rect.z, rect.y, rect.w);
		context->drawRectangle(rect.get(), color.get(), color.a, lineWidth);
	}

	static __forceinline void fillRectangle(Rect rect, Color color) {
		rect = Rect(rect.x, rect.z, rect.y, rect.w);
		context->fillRectangle(rect.get(), color.get(), color.a);
	}

	static __forceinline void renderOutlinedText(std::string text, Vec2<float> textPos, Color fillColor, Color outlineColor, float textSize, Font* font) {
		// Calculate how much to go forward/backwards based on text size
		float backwards = -0.25f * textSize;
		float forwards = 0.5f * textSize;
		// Get rid of formatting
		std::string rt = text;
		int formatPos = rt.find("�");
		while (formatPos != std::string::npos) {
			if (rt.size() > formatPos + 2) {
				if (rt.at(formatPos + 2) != 'l') { // Don't get rid of bold
					rt.erase(formatPos - 1, 3);
				}
			}
			formatPos = rt.find("�", formatPos + 1);
		}
		// Outline the text
		RenderUtils::drawText(font, textPos + Vec2<float>(backwards, backwards), rt, outlineColor, textSize);
		RenderUtils::drawText(font, textPos + Vec2<float>(forwards, forwards), rt, outlineColor, textSize);
		RenderUtils::drawText(font, textPos + Vec2<float>(backwards, forwards), rt, outlineColor, textSize);
		RenderUtils::drawText(font, textPos + Vec2<float>(forwards, backwards), rt, outlineColor, textSize);
		RenderUtils::flush();

		// Draw text
		RenderUtils::drawText(font, textPos, text, fillColor, textSize);
		RenderUtils::flush();
	}

	static ImVec2 scaledResToReal(Vec2<float> scaledRes) {
		float scaledSize = *Minecraft::getClientInstance()->getGuiData()->getScale();

		return ImVec2(scaledRes.x * scaledSize, scaledRes.y * scaledSize);
	}

	static Vec2<float> realResToScaled(ImVec2 realRes) {
		auto data = Minecraft::getClientInstance()->getGuiData();
		ImVec2 actualRes = ImVec2(data->actualRes.x, data->actualRes.y);
		ImVec2 res = ImVec2(data->res.x, data->res.y);
		return Vec2<float>(realRes.x * (res.x / actualRes.x), realRes.y * (res.y / actualRes.y));
	}

	static Vec2<float> getScreenResolution() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		return Vec2<float>(desktop.right, desktop.bottom);
	}
};
extern RenderUtils* renderUtils;
