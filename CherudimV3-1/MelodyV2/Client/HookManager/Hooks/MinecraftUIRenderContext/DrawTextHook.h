#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/MinecraftUIRenderContext.h"
#include "../../../../SDK/Classes/TextHolder.h"
#include "../../../../Utils/Math.h"
#include "../../../../Utils/ColorUtils.h"

#include "../../../Client.h"

class DrawTextHook {
protected:
	using func_t = __int64(__fastcall*)(MinecraftUIRenderContext*, FontBitmap*, float*, std::string*, MC_Color*, float, unsigned int, TextMeasureData*, CaretMeasureData*);
	static inline func_t func;

	static __int64 drawTextCallBack(MinecraftUIRenderContext* _this, FontBitmap* font, float* pos, std::string* text, MC_Color* color, float alpha, unsigned int textAlignment, TextMeasureData* textMeasureData, CaretMeasureData* caretMeasureData) {
		/*
		static std::string test = "Melody";
		
		std::string textStr = *text;

		std::vector<int> posList;
		bool shouldCancel = false;
		for (int i = 0; i < INT_MAX; i++) { // Minecraft ncc
			if (textStr[i] == '\0') break;
			std::string current;
			for (int j = 0; j < test.size(); j++) {
				current += textStr[i + j];
			}
			if (strcmp(current.c_str(), test.c_str()) == 0) {
				posList.push_back(i);
				shouldCancel = true;
			}
		}
		
		Vec2<float> tPos = Vec2<float>(pos[0], pos[2]);
		for (int i2 = 0; i2 < INT_MAX; i2++) { // Minecraft ncc
			if (textStr[i2] == '\0') break;
			std::string cStr = "A";
			if (posList.size() != 0 && i2 == posList[0]) {
				for (int j = 0; j < test.size(); j++) {
					cStr[0] = textStr[i2];
					RenderUtils::drawText(tPos, cStr, ColorUtils::getRainbowColor(3.f, 1.f, 1.f, -i2 * 25).toMC_Color(), textMeasureData->textSize, alpha, textMeasureData->showShadow);
					tPos.x += RenderUtils::getTextWidth(cStr, textMeasureData->textSize);
					if (j != test.size() - 1) i2++;
				}
				posList.erase(posList.begin());
			}
			else if (shouldCancel) {
				cStr[0] = textStr[i2];
				RenderUtils::drawText(tPos, cStr, *color, textMeasureData->textSize, alpha, textMeasureData->showShadow);
				tPos.x += RenderUtils::getTextWidth(cStr, textMeasureData->textSize);
			}
		}
		if (shouldCancel) return 0;
		*/
		return func(_this, font, pos, text, color, alpha, textAlignment, textMeasureData, caretMeasureData);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("DrawTextHook", address, (void*)&DrawTextHook::drawTextCallBack, (void*)&func);
	}
};